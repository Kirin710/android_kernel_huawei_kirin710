/* Copyright (c) 2011-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "hw_flash.h"
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/consumer.h>
#include <linux/power/hisi_hi6521_charger_power.h>
#include <linux/bitops.h>
#include <sound/hi6555c_common.h>
#include <securec.h>


#define FLASH_LED_MAX			(16)
#define TORCH_LED_MAX			(8)
#define FLASH_TIMEOUT_MS        (600)

#define SCHG_FLASH_MAX_CUR                    (1500)  //1500ma
#define SCHG_TORCH_MAX_CUR                   (400)    //400ma
#define SCHG_TORCH_DEFAULT_CUR           (150)    //150ma
#define SCHG_MA_TO_UM                              (1000)

#define SCHG_BOOST_REGULATOR      "pvdd-classd"    /*"schg_boost3"*/
#define SCHG_FLASH_MODE_REGULATOR "flash-led" /*"schg_source1"*/
#define SCHG_TORCH_MODE_REGULATOR "torch-led" /*"schg_source2"*/

struct hw_scharger_private_data_t {
    unsigned int flash_led[FLASH_LED_MAX];
    unsigned int torch_led[TORCH_LED_MAX];
    unsigned int flash_led_num;
    unsigned int torch_led_num;
    struct regulator *flash_inter_ldo;
    struct regulator *flash_mode_ldo;
    struct regulator *torch_mode_ldo;
    unsigned int status;
};

/* Internal varible define */
static struct hw_scharger_private_data_t hw_scharger_pdata;
static struct hw_flash_ctrl_t hw_scharger_ctrl;

//whether need to mute audio when flash
static unsigned int audio_codec_mute_flag = 0;

DEFINE_HISI_FLASH_MUTEX(scharger);

#ifdef CAMERA_FLASH_FACTORY_TEST
extern int register_camerafs_attr(struct device_attribute *attr);
#endif

static int hw_scharger_init(struct hw_flash_ctrl_t *flash_ctrl)
{
    struct hw_scharger_private_data_t *pdata = (struct hw_scharger_private_data_t *)flash_ctrl->pdata;
    if (NULL == pdata) {
        cam_err("%s pdata is NULL", __func__);
        return -1;
    }

    pdata->status = STANDBY_MODE;

    return 0;
}

static int hw_scharger_exit(struct hw_flash_ctrl_t *flash_ctrl)
{
    return 0;
}

static int hw_scharger_flash_mode(struct hw_flash_ctrl_t *flash_ctrl, int data)
{
    struct hw_scharger_private_data_t *pdata = NULL;
    int ret = 0;

    cam_info("%s data=%d.\n", __func__, data);

    if (NULL == flash_ctrl) {
        cam_err("%s flash_ctrl is NULL.", __func__);
        return -1;
    }

    pdata = (struct hw_scharger_private_data_t *)flash_ctrl->pdata;

    if(data <= 0) {
        cam_err("set the flash_lum_level: %d", data);
        return -1;
    }

    if (data > SCHG_FLASH_MAX_CUR) {
        cam_warn("set the max flash_lum_level: %d", data);
        data = SCHG_FLASH_MAX_CUR;
    }

    if ((NULL == pdata->flash_inter_ldo) || (NULL == pdata->flash_mode_ldo)) {
        cam_err("%s regulator is NULL", __func__);
        return -1;
    }

    /*if flash has already on do nothing*/
    if(BIT(FLASH_MODE) & pdata->status){
        cam_info("%s already in flash mode, do nothing",__func__);
        return 0;
    }

    if (audio_codec_mute_flag){
        audio_codec_mute_pga(1);//lint !e64
    }

    ret = scharger_flash_led_timeout_config(FLASH_TIMEOUT_MS);
    if (ret < 0) {
        cam_err("%s scharger_flash_led_timeout_config  fail ret = %d ", __func__, ret);
        goto err_out;
    }

    ret = scharger_flash_led_timeout_enable();
    if (ret < 0) {
        cam_err("%s scharger_flash_led_timeout_enable  fail ret = %d ", __func__, ret);
        goto err_out;
    }
    ret = regulator_enable(pdata->flash_inter_ldo);
    if (ret < 0) {
        cam_err("%s regulator_enable flash_inter_ldo fail ret = %d", __func__, ret);
        goto err_out;
    }
    udelay(500);

    //the scharger current unit is ua
    data = data *SCHG_MA_TO_UM;

    ret = regulator_set_current_limit(pdata->flash_mode_ldo, data, data);
    if (ret < 0) {
        cam_err("%s regulator_set_current_limit  fail ret = %d current is %d", __func__, ret, data);
        if (0 != regulator_disable(pdata->flash_inter_ldo)) {
            cam_notice("Failed: flash_inter_ldo regulator_disable\n");
        }
        goto err_out;
    }

    ret = regulator_enable(pdata->flash_mode_ldo);
    if (ret < 0) {
        cam_err("%s regulator_enable torch_mode_ldo fail ret = %d", __func__, ret);
        if (0 != regulator_disable(pdata->flash_inter_ldo)) {
            cam_notice("Failed: flash_inter_ldo regulator_disable\n");
        }
        goto err_out;
    }

    pdata->status |= BIT(FLASH_MODE);
    return 0;

err_out:

    if (audio_codec_mute_flag){
        audio_codec_mute_pga(0);//lint !e64
    }

    return ret;

}

static int hw_scharger_torch_mode(struct hw_flash_ctrl_t *flash_ctrl, int data)
{
    struct hw_scharger_private_data_t *pdata = NULL;
    int ret = 0;

    cam_info("%s data=%d.\n", __func__, data);

    if (NULL == flash_ctrl) {
        cam_err("%s flash_ctrl is NULL.", __func__);
        return -1;
    }

    pdata = (struct hw_scharger_private_data_t *)flash_ctrl->pdata;

    if(data <= 0) {
        cam_err("set the torch_lum_level: %d", data);
        return -1;
    }

    if (data > SCHG_TORCH_MAX_CUR) {
        cam_warn("set the max torch_lum_level: %d", data);
        data = SCHG_TORCH_MAX_CUR;
    }

    if ((NULL == pdata->flash_inter_ldo) || (NULL == pdata->torch_mode_ldo)) {
        cam_err("%s regulator is NULL", __func__);
        return -1;
    }

    /*if flash has already on do nothing*/
    if(BIT(TORCH_MODE) & pdata->status){
        cam_info("%s already in torch mode, do nothing",__func__);
        return 0;
    }

    ret = regulator_enable(pdata->flash_inter_ldo);
    if (ret < 0) {
        cam_err("%s regulator_enable flash_inter_ldo fail ret = %d", __func__, ret);
        return ret;
    }

    udelay(500);

    //the scharger current unit is ua
    data = data *SCHG_MA_TO_UM;

    ret = regulator_set_current_limit(pdata->torch_mode_ldo, data, data);
    if (ret < 0) {
        cam_err("%s regulator_set_current_limit  fail ret = %d current is %d", __func__, ret, data);
        return ret;
    }

    ret = regulator_enable(pdata->torch_mode_ldo);
    if (ret < 0) {
        cam_err("%s regulator_enable torch_mode_ldo fail ret = %d", __func__, ret);
        if (0 != regulator_disable(pdata->flash_inter_ldo)) {
            cam_notice("Failed: flash_inter_ldo regulator_disable\n");
        }
        return ret;
    }
    pdata->status |= BIT(TORCH_MODE);
    return 0;
}

static int hw_scharger_on(struct hw_flash_ctrl_t *flash_ctrl, void *data)
{
    struct hw_flash_cfg_data *cdata = (struct hw_flash_cfg_data *)data;
    int rc = -1;

    if ((NULL == flash_ctrl) || (NULL == cdata)) {
        cam_err("%s flash_ctrl or cdata is NULL.", __func__);
        return rc;
    }

    cam_debug("%s mode=%d, level=%d.\n", __func__, cdata->mode, cdata->data);

    mutex_lock(flash_ctrl->hw_flash_mutex);

    if (FLASH_MODE == cdata->mode) {
        rc = hw_scharger_flash_mode(flash_ctrl, cdata->data);
    } else {
        rc = hw_scharger_torch_mode(flash_ctrl, cdata->data);
    }
    flash_ctrl->state.mode = cdata->mode;
    flash_ctrl->state.data = cdata->data;
    mutex_unlock(flash_ctrl->hw_flash_mutex);

    return rc;
}

static int hw_scharger_off(struct hw_flash_ctrl_t *flash_ctrl)
{
    struct hw_scharger_private_data_t *pdata = NULL;

    cam_debug("%s enter.\n", __func__);

    if (NULL == flash_ctrl) {
        cam_err("%s flash_ctrl is NULL.", __func__);
        return -1;
    }

    if (STANDBY_MODE == flash_ctrl->state.mode) {
        cam_notice("%s flash led has been powered off.", __func__);
        return 0;
    }

    pdata = (struct hw_scharger_private_data_t *)flash_ctrl->pdata;

    if ((NULL == pdata->flash_inter_ldo)
            || (NULL == pdata->flash_mode_ldo)
            || (NULL == pdata->torch_mode_ldo)) {
        cam_err("%s regulator is NULL", __func__);
        return -1;
    }

    mutex_lock(flash_ctrl->hw_flash_mutex);

    if (BIT(TORCH_MODE) & pdata->status) {
        if (0 != regulator_disable(pdata->torch_mode_ldo)) {
            cam_notice("Failed: torch_mode_ldo regulator_disable\n");
        }
        if (0 != regulator_disable(pdata->flash_inter_ldo)) {
            cam_notice("Failed: flash_inter_ldo regulator_disable\n");
        }
        pdata->status &= ~BIT(TORCH_MODE);
    }

    if (BIT(FLASH_MODE) & pdata->status) {
        if (0 != regulator_disable(pdata->flash_mode_ldo)) {
            cam_notice("Failed: flash_mode_ldo regulator_disable\n");
        }
        if (0 != regulator_disable(pdata->flash_inter_ldo)) {
            cam_notice("Failed: flash_inter_ldo regulator_disable\n");
        }
        pdata->status &= ~BIT(FLASH_MODE);
    }

    if (audio_codec_mute_flag){
        audio_codec_mute_pga(0);//lint !e64
    }

    flash_ctrl->state.mode = STANDBY_MODE;
    flash_ctrl->state.data = 0;

    mutex_unlock(flash_ctrl->hw_flash_mutex);

    return 0;
}

static int hw_scharger_get_dt_data(struct hw_flash_ctrl_t *flash_ctrl)
{
    struct hw_scharger_private_data_t *pdata = NULL;
    struct device_node *dev_node = NULL;
    unsigned int i;
    int rc = -1;

    cam_debug("%s enter.\n", __func__);

    if (NULL == flash_ctrl) {
        cam_err("%s flash_ctrl is NULL.", __func__);
        return rc;
    }

    pdata = (struct hw_scharger_private_data_t *)flash_ctrl->pdata;
    dev_node= flash_ctrl->dev->of_node;

    rc = of_property_read_u32(dev_node, "huawei,flash_led_num", &pdata->flash_led_num);
    cam_debug("%s hisi,flash_led_num %d, rc %d\n", __func__, pdata->flash_led_num, rc);
    if (rc < 0) {
        cam_err("%s failed %d\n", __func__, __LINE__);
        return rc;
    }

    rc = of_property_read_u32(dev_node, "huawei,torch_led_num",
            &pdata->torch_led_num);
    cam_debug("%s hisi,torch_led_num %d, rc %d\n", __func__,
            pdata->torch_led_num, rc);
    if (rc < 0) {
        cam_err("%s failed %d\n", __func__, __LINE__);
        return rc;
    }

    rc = of_property_read_u32_array(dev_node, "huawei,flash_led",
            pdata->flash_led, pdata->flash_led_num);
    if (rc < 0) {
        cam_err("%s failed line %d\n", __func__, __LINE__);
        return rc;
    } else {
        for (i=0; i< pdata->flash_led_num; i++) {
            cam_debug("%s flash_led[%d]=0x%x.\n", __func__, i,
                    pdata->flash_led[i]);
        }
    }

    rc = of_property_read_u32_array(dev_node, "huawei,torch_led",
            pdata->torch_led, pdata->torch_led_num);
    if (rc < 0) {
        cam_err("%s failed line %d\n", __func__, __LINE__);
        return rc;
    } else {
        for (i = 0; i < pdata->torch_led_num; i++) {
            cam_debug("%s torch_led[%d]=0x%x.\n", __func__, i,
                    pdata->torch_led[i]);
        }
    }

    rc = of_property_read_u32(dev_node, "huawei,audio_codec_mute_flag",
            &audio_codec_mute_flag);
    cam_info("%s hisi,audio_codec_mute_flag %d, rc %d\n", __func__,
            audio_codec_mute_flag, rc);
    if (rc < 0) {
        cam_err("%s failed %d\n", __func__, __LINE__);
    }

    return rc;
}

#ifdef CAMERA_FLASH_FACTORY_TEST
static ssize_t hw_scharger_lightness_show(struct device *dev,
    struct device_attribute *attr,char *buf)
{
	int rc;

	rc = snprintf_s(buf, MAX_ATTRIBUTE_BUFFER_SIZE,
		MAX_ATTRIBUTE_BUFFER_SIZE - 1,
		"mode=%d, data=%d.\n",
		hw_scharger_ctrl.state.mode, hw_scharger_ctrl.state.data);
	if (rc <= 0) {
		cam_err("%s, %d, snprintf_s return error %d",
			__func__, __LINE__, rc);
		return -EINVAL;
	}
	return rc;
}

static int hw_scharger_param_check(char *buf, unsigned long *param,
    int num_of_par)
{
    char *token;
    int base, cnt;

    token = strsep(&buf, " ");

    for (cnt = 0; cnt < num_of_par; cnt++)
    {
        if (token != NULL)
        {
            if ((token[1] == 'x') || (token[1] == 'X')) {
                base = 16;
            } else {
                base = 10;
            }
            if (strict_strtoul(token, base, &param[cnt]) != 0) {
                return -EINVAL;
            }

            token = strsep(&buf, " ");
        }
        else
        {
            return -EINVAL;
        }
    }
    return 0;
}

static ssize_t hw_scharger_lightness_store(struct device *dev,
    struct device_attribute *attr, const char *buf, size_t count)
{
    struct hw_flash_cfg_data cdata = {0};
    unsigned long param[2] = {0};
    int rc = 0;
    struct hw_scharger_private_data_t *pdata = (struct hw_scharger_private_data_t *)(hw_scharger_ctrl.pdata);

    cam_info("%s enter,buf=%s.", __func__,buf);
    rc = hw_scharger_param_check((char *)buf, param, 2);
    if (rc < 0) {
        cam_err("%s failed to check param.", __func__);
        return rc;
    }

    char flash_id = (int)param[0];
    cdata.mode = (int)param[1];
    cam_info("%s flash_id=%d,cdata.mode=%d.", __func__, flash_id, cdata.mode);
    if(1 != flash_id){//bit[0]- rear first flash light.bit[1]- rear sencond flash light.bit[2]- front flash light; dallas product using only rear first flash light
        cam_err("%s scharger wrong flash_id=%d.", __func__,flash_id);
        return -1;
    }

    if (cdata.mode == STANDBY_MODE) {
        rc = hw_scharger_off(&hw_scharger_ctrl);
        if (rc < 0) {
            cam_err("%s scharger flash off error.", __func__);
            return rc;
        }
    } else if(cdata.mode == TORCH_MODE){
        cdata.data = SCHG_TORCH_DEFAULT_CUR;
        cam_info("%s mode=%d, max_current=%d.", __func__, cdata.mode, cdata.data);

        rc = hw_scharger_on(&hw_scharger_ctrl, &cdata);
        if (rc < 0) {
            cam_err("%s scharger flash on error.", __func__);
            return rc;
        }
    } else {
        cam_err("%s scharger wrong mode=%d.", __func__,cdata.mode);
        return -1;
    }

    return count;
}
#endif

static ssize_t hw_scharger_flash_mask_show(struct device *dev,
    struct device_attribute *attr,char *buf)
{
    int rc = 0;

    snprintf_s(buf, MAX_ATTRIBUTE_BUFFER_SIZE, MAX_ATTRIBUTE_BUFFER_SIZE-1,  "flash_mask_disabled=%d.\n",
            hw_scharger_ctrl.flash_mask_enable);
    rc = strlen(buf)+1;
    return rc;
}

static ssize_t hw_scharger_flash_mask_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
    if ('0' == buf[0]) {
        hw_scharger_ctrl.flash_mask_enable = false;
    } else {
        hw_scharger_ctrl.flash_mask_enable = true;
    }
    cam_debug("%s flash_mask_enable=%d.", __func__,
            hw_scharger_ctrl.flash_mask_enable);
    return count;
}

static void hw_scharger_torch_brightness_set(struct led_classdev *cdev,
        enum led_brightness brightness)
{
    struct hw_flash_cfg_data cdata;
    int rc;
    unsigned int led_bright = brightness;

    if (STANDBY_MODE == led_bright) {
        rc = hw_scharger_off(&hw_scharger_ctrl);
        if (rc < 0) {
            cam_err("%s scharger off error.", __func__);
            return;
        }
    } else {
        cdata.mode = TORCH_MODE;
        cdata.data = SCHG_TORCH_DEFAULT_CUR;
        rc = hw_scharger_on(&hw_scharger_ctrl, &cdata);
        if (rc < 0) {
            cam_err("%s scharger on error.", __func__);
            return;
        }
    }
}

#ifdef CAMERA_FLASH_FACTORY_TEST
static struct device_attribute hw_scharger_lightness =
    __ATTR(flash_lightness, 0664, hw_scharger_lightness_show, hw_scharger_lightness_store);
#endif

static struct device_attribute hw_scharger_flash_mask =
    __ATTR(flash_mask, 0664, hw_scharger_flash_mask_show, hw_scharger_flash_mask_store);

static int hw_scharger_register_attribute(struct hw_flash_ctrl_t *flash_ctrl,
    struct device *dev)
{
    int rc = 0;
    struct hw_scharger_private_data_t *pdata = NULL;

    if ((NULL == flash_ctrl) || (NULL == dev)) {
        cam_err("%s flash_ctrl or dev is NULL.", __func__);
        return -1;
    }

    pdata = (struct hw_scharger_private_data_t *)(flash_ctrl->pdata);
    if(NULL == pdata) {
        cam_err("%s pdata is NULL.", __func__);
        return -1;
    }

    flash_ctrl->cdev_torch.name = "torch";
    flash_ctrl->cdev_torch.max_brightness = (enum led_brightness)pdata->torch_led_num;
    flash_ctrl->cdev_torch.brightness_set = hw_scharger_torch_brightness_set;
    rc = led_classdev_register((struct device *)dev, &flash_ctrl->cdev_torch);
    if (rc < 0) {
        cam_err("%s failed to register torch classdev.", __func__);
        goto err_out;
    }
#ifdef CAMERA_FLASH_FACTORY_TEST
    rc = device_create_file(dev, &hw_scharger_lightness);
    if (rc < 0) {
        cam_err("%s failed to creat lightness attribute.", __func__);
        goto err_create_lightness_file;
    }
#endif
    rc = device_create_file(dev, &hw_scharger_flash_mask);
    if (rc < 0) {
        cam_err("%s failed to creat flash_mask attribute.", __func__);
        goto err_create_flash_mask_file;
    }

    return 0;

err_create_flash_mask_file:
#ifdef CAMERA_FLASH_FACTORY_TEST
    device_remove_file(dev, &hw_scharger_lightness);
err_create_lightness_file:
#endif
    led_classdev_unregister(&flash_ctrl->cdev_torch);
err_out:

    return rc;
}

static int hw_scharger_match(struct hw_flash_ctrl_t *flash_ctrl)
{
    cam_info("%s ernter.\n", __func__);
#ifdef CAMERA_FLASH_FACTORY_TEST
    register_camerafs_attr(&hw_scharger_lightness);
#endif
    return 0;
}

static const struct of_device_id hw_scharger_match_id[] = {
	{.compatible = "huawei,scharger_flash"},
	{}
};

MODULE_DEVICE_TABLE(of, scharger_dt_match);

static int hw_scharger_probe(struct platform_device *pdev)
{
    int ret = 0;
    struct regulator *ldo = NULL;

    ret = memset_s(&hw_scharger_pdata, sizeof(hw_scharger_pdata), 0, sizeof(hw_scharger_pdata));
    if (ret != EOK) {
        cam_err("%s. failed to memset_s write_data.", __func__);
    }
    ldo = devm_regulator_get(&pdev->dev, SCHG_BOOST_REGULATOR);
    if (IS_ERR_OR_NULL(ldo)) {
        cam_err("%s: Could not get regulator : %s\n", __func__, SCHG_BOOST_REGULATOR);
        ret = -ENXIO;
        goto fail;
    }

    hw_scharger_pdata.flash_inter_ldo = ldo;

    /* get flash mode regulator */
    ldo = devm_regulator_get(&pdev->dev, SCHG_FLASH_MODE_REGULATOR);
    if (IS_ERR_OR_NULL(ldo)) {
        cam_err("%s: Could not get regulator : %s\n", __func__, SCHG_FLASH_MODE_REGULATOR);
        ret =  -ENXIO;
        goto fail;
    }
    hw_scharger_pdata.flash_mode_ldo = ldo;

    /* get torch mode regulator */
    ldo = devm_regulator_get(&pdev->dev, SCHG_TORCH_MODE_REGULATOR);
    if (IS_ERR_OR_NULL(ldo)) {
        cam_err("%s: Could not get regulator : %s\n", __func__, SCHG_TORCH_MODE_REGULATOR);
        ret =  -ENXIO;
        goto fail;
    }

    hw_scharger_pdata.torch_mode_ldo = ldo;

    hw_scharger_ctrl.pdata = &hw_scharger_pdata;
    platform_set_drvdata(pdev, &hw_scharger_ctrl);

    return hw_flash_platform_probe(pdev, &hw_scharger_ctrl);

fail:
    if (NULL != hw_scharger_pdata.flash_inter_ldo) {
        devm_regulator_put(hw_scharger_pdata.flash_inter_ldo);
        hw_scharger_pdata.flash_inter_ldo = NULL;
    }
    if (NULL != hw_scharger_pdata.flash_mode_ldo) {
        devm_regulator_put(hw_scharger_pdata.flash_mode_ldo);
        hw_scharger_pdata.flash_mode_ldo = NULL;
    }

    return ret;
}

static int hw_scharger_remove(struct platform_device *pdev)
{
    return hw_scharger_ctrl.func_tbl->flash_exit(&hw_scharger_ctrl);
}

static struct platform_driver hw_scharger_platform_driver = {
    .probe = hw_scharger_probe,
    .remove = hw_scharger_remove,
    .driver = {
        .name = "scharger",
        .owner = THIS_MODULE,
#ifdef CONFIG_OF
        .of_match_table = of_match_ptr(hw_scharger_match_id),
#endif
    },
};

static struct hw_flash_fn_t hw_scharger_func_tbl = {
    .flash_config = hw_flash_config,
    .flash_init = hw_scharger_init,
    .flash_exit = hw_scharger_exit,
    .flash_on = hw_scharger_on,
    .flash_off = hw_scharger_off,
    .flash_match = hw_scharger_match,
    .flash_get_dt_data = hw_scharger_get_dt_data,
    .flash_register_attribute = hw_scharger_register_attribute,
};

static struct hw_flash_ctrl_t hw_scharger_ctrl = {//lint !e31
    .func_tbl = &hw_scharger_func_tbl,
    .hw_flash_mutex = &flash_mut_scharger,
    .pdata = (void*)&hw_scharger_pdata,
    .flash_mask_enable = true,
    .state = {
        .mode = STANDBY_MODE,
    },
};

module_platform_driver(hw_scharger_platform_driver);

MODULE_DESCRIPTION("SCHARGER CAMERA FLASH");
MODULE_LICENSE("GPL v2");
