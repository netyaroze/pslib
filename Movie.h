

#ifndef _MOVIE_H_
#define _MOVIE_H_

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <r3000.h>
#include <asm.h>
#include <libapi.h>
#include <libpress.h>

// Play a Movie. PlayMovie(Movie Filename on CDROM, Movie Frames Number);
void	PlayMovie(char *name, u_long nframes);

#endif
