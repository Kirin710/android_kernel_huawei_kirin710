/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*****************************************************************************/
/*                                                                           */
/*                Copyright 1999 - 2003, Huawei Tech. Co., Ltd.              */
/*                           ALL RIGHTS RESERVED                             */
/*                                                                           */
/* FileName: v_sprintf.c                                                     */
/*                                                                           */
/* Author:                                                                   */
/*                                                                           */
/* Version: 1.0                                                              */
/*                                                                           */
/* Date:                                                                     */
/*                                                                           */
/* Description: copy this file from Dopra                                    */
/*                                                                           */
/*                                                                           */
/* Others:                                                                   */
/*                                                                           */
/* History:                                                                  */
/* 1. Date:                                                                  */
/*    Author:                                                                */
/*    Modification:                                                          */
/*                                                                           */
/*****************************************************************************/


#include "v_IO.h"
#include "v_private.h"

#if (VOS_OS_VER != VOS_WIN32)
#include <securec.h>
#endif

#if (VOS_OS_VER == VOS_WIN32)
#include "ut_mem.h"
#endif

/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_V_SPRINTF_C

#define LENGTH_OF_PRINT_LINE_BUF        (1024)

/*****************************************************************************
 Function   : vos_assert
 Description: Report an assert
 Input      : pcFileName -- File name of this assert occuring
              ulLineNo   -- Line number of this assert occuring
 Output     : None
 Return     : None
 *****************************************************************************/
MODULE_EXPORTED VOS_VOID vos_assert( VOS_UINT32 ulFileID, VOS_INT LineNo)
{
    /*lint -e813 */
    VOS_CHAR vos_PrintBuf[LENGTH_OF_PRINT_LINE_BUF];
    /*lint +e813 */
    VOS_INT sprinfResult;

    VOS_CHAR *String = "Assert";

    sprinfResult = snprintf_s(vos_PrintBuf,
                              LENGTH_OF_PRINT_LINE_BUF,
                              LENGTH_OF_PRINT_LINE_BUF - 1,
                              "%s File: %d, Line: %d",
                              String,
                              ulFileID,
                              LineNo);

    if (sprinfResult <= VOS_NULL)
    {
        return;
    }

    vos_printf("\n[PAM][OSA] %s.\r\n",vos_PrintBuf);

    return;
}




