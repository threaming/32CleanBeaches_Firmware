/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         util collection
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          10.03.2020
 *
 * This file contains as well as modified functions from the utility
 * component of processor expert:
 *
 * Copyright(c) 2014 - 2017, Erich Styger
 * Web : https://mcuoneclipse.com
 * SourceForge: https://sourceforge.net/projects/mcuoneclipse
 * Git : https ://github.com/ErichStyger/McuOnEclipse_PEx
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met :
 *
 * -Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and / or
 * other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_UTIL_H_
#define SOURCES_UTILS_UTIL_H_

#include <stddef.h>

void utilStrcpy(char *dst, size_t dstSize, const char *src);
void utilStrcat(char *dst, size_t dstSize, const char *src);

void utilNum16uToStr(char *dst, uint16_t dstSize, uint16_t num);
void utilNum16sToStr(char *dst, size_t dstSize, int16_t val);

void utilNum32uToStr(char *dst, size_t dstSize, uint32_t num);
void utilNum32sToStr(char *dst, size_t dstSize, int32_t val);

void utilStrcatNum16u(char *dst, size_t dstSize, uint16_t num);
void utilStrcatNum16s(char *dst, size_t dstSize, int16_t num);

void utilStrcatNum32u(char *dst, size_t dstSize, uint32_t val);
void utilStrcatNum32s(char *dst, size_t dstSize, int32_t val);

tError utilScanDecimal16u(const char **str, uint16_t *value);
tError utilScanDecimal16s(const char **str, int16_t *value);

void utilWaitUs(uint16_t us);

#endif /* SOURCES_UTILS_UTIL_H_ */
