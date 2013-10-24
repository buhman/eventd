#include <alsa/asoundlib.h>

int
eventd_get_mixer_elem(const char *hctl_name,
		      const char *selem_name,
		      snd_mixer_elem_t **elem)
{
  int err;
  
  snd_mixer_selem_id_t *sid;
  snd_mixer_t *mixer_handle;

  {
    err = snd_mixer_open(&mixer_handle, 0);
    if (err < 0) {
      printf("snd_mixer_open(): %s\n", snd_strerror(err));
      return err;
    }

    err = snd_mixer_attach(mixer_handle, hctl_name);
    if (err < 0) {
      printf("snd_mixer_attach(): %s\n", snd_strerror(err));
      return err;
    }
  
    err = snd_mixer_selem_register(mixer_handle, NULL, NULL);
    if (err < 0) {
      printf("snd_mixer_selem_register(): %s\n", snd_strerror(err));
      return err;
    }
    
    err = snd_mixer_load(mixer_handle);
    if (err < 0) {
      printf("snd_mixer_load(): %s\n", snd_strerror(err));
      return err;
    }
  } /* ... */
  
  {
    snd_mixer_selem_id_alloca(&sid);

    snd_mixer_selem_id_set_name(sid, selem_name);

    *elem = snd_mixer_find_selem(mixer_handle, sid);
    if (!(*elem)) {
      printf("snd_mixer_find_selem(): NULL\n");
      return -1;
    }
  } /* ... */

  printf("snd_mixer_selem_id: %s/%s\n", hctl_name, selem_name);
  
  return 0;
}

int
eventd_elem_switch_toggle(snd_mixer_elem_t *elem)
{
  int err, value;

  err = snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_UNKNOWN,
				      &value);
  if (err < 0) {
    printf("snd_mixer_selem_get_playback_switch(): %s\n", snd_strerror(err));
    return err;
  }

  printf("switch: %d -> %d\n", value, value ? 0 : 1);
  value = value ? 0 : 1;
  
  err = snd_mixer_selem_set_playback_switch_all(elem, value);
  if (err < 0) {
    printf("snd_mixer_selem_set_playback_switch_all(): %s\n", snd_strerror(err));
    return err;
  }
  return 0;
}

int
eventd_elem_volume_offset(snd_mixer_elem_t *elem,
			   int offset)
{
  long volume, max, min;
  int err;

  err = snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
  if (err < 0) {
    printf("snd_mixer_selem_get_playback_volume_range(): %s\n",
	   snd_strerror(err));
    return err;
  }
  
  err = snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_UNKNOWN,
					    &volume);
  if (err < 0) {
    printf("snd_mixer_selem_get_playback_volume(): %s\n", snd_strerror(err));
    return err;
  }

  printf("volume: %lu += %d\n", volume, offset);
  volume += offset;

  if ((volume > min) && (volume < max)) {
    err = snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_UNKNOWN,
					      volume);
    if (err < 0) {
      printf("snd_mixer_selem_set_playback_volume(): %s\n", snd_strerror(err));
      return err;
    }

    err = snd_mixer_selem_set_playback_switch_all(elem, 1);
    if (err < 0) {
      printf("snd_mixer_selem_set_playback_switch_all(): %s\n", snd_strerror(err));
      return err;
    }
  }
  return 0;
}
