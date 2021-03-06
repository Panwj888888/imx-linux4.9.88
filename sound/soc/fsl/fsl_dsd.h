/*
 * Copyright 2018 NXP
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __FSL_DSD_H
#define __FSL_DSD_H

#include <linux/pinctrl/consumer.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

static bool fsl_is_dsd(struct snd_pcm_hw_params *params)
{
	snd_pcm_format_t format = params_format(params);

	switch (format) {
	case SNDRV_PCM_FORMAT_DSD_U8:
	case SNDRV_PCM_FORMAT_DSD_U16_LE:
	case SNDRV_PCM_FORMAT_DSD_U16_BE:
	case SNDRV_PCM_FORMAT_DSD_U32_LE:
	case SNDRV_PCM_FORMAT_DSD_U32_BE:
		return true;
	default:
		return false;
	}
}

static struct pinctrl_state *fsl_get_pins_state(struct pinctrl *pinctrl,
	struct snd_pcm_hw_params *params)
{
	int dsd_bclk;
	struct pinctrl_state *state = 0;

	if (fsl_is_dsd(params)) {
		dsd_bclk = params_rate(params) * params_physical_width(params);

		switch (dsd_bclk) {
		case 22579200: /* DSD512 */
			state = pinctrl_lookup_state(pinctrl, "dsd512");
			break;
		}

		/* Get default DSD state */
		if (IS_ERR_OR_NULL(state))
			state = pinctrl_lookup_state(pinctrl, "dsd");
	}

	/* Get default state */
	if (IS_ERR_OR_NULL(state))
		state = pinctrl_lookup_state(pinctrl, "default");

	return state;
}

#endif /* __FSL_DSD_H */
