/* Copyright Justin Mammarella jmamma@gmail.com 2018 */

#ifndef AUXPAGES_H__
#define AUXPAGES_H__

extern MCLEncoder mixer_param1(0, 127);
extern MCLEncoder mixer_param2(0, 127);
extern MCLEncoder mixer_param3(0, 8);
MixerPage mixer_page(&mixer_param1, &mixer_param2, &mixer_param3);
CuePage cue_page(&mixer_param1, &mixer_param2, &mixer_param3);
#endif /* AUXPAGES_H__ */