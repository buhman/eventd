#include <alsa/asoundlib.h>

int
eventd_get_mixer_elem(const char *hctl_name,
		      const char *elem_name,
		      snd_mixer_t **mixer,
		      snd_mixer_elem_t **elem);

int
eventd_elem_switch_toggle(snd_mixer_elem_t *elem);

int
eventd_elem_volume_offset(snd_mixer_elem_t *elem,
			  int inc);

