// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef __SENSORS_H
#define __SENSORS_H


#ifdef __cplusplus
extern "C" {
#endif
float getTemperature(void);
float getHumidity(void);
float getLight(void);
float getDust(void);
float getNoise(void);
void sensorsSetup(void);
void sensorsLoop(void);

#ifdef __cplusplus
}
#endif


#endif//__SENSORS_H
