/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2009 Chris Wilson
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is Chris Wilson.
 *
 * Contributor(s):
 *      Chris Wilson <chris@chris-wilson.co.uk>
 */


#include "cairoint.h"
#include "cairo-xboot.h"

struct cairo_xboot_surface_t {
	void * pixels;
	int width;
	int height;
	cairo_format_t fmt;

	struct surface_t * surface;
	cairo_surface_t * cs;
};

static cairo_format_t cairo_format_from_pixel_format (enum pixel_format fmt)
{
	switch(fmt)
	{
	case PIXEL_FORMAT_ARGB_8888:
		return CAIRO_FORMAT_ARGB32;

	case PIXEL_FORMAT_RGB_888:
		return CAIRO_FORMAT_RGB24;

	case PIXEL_FORMAT_RGB_565:
		return CAIRO_FORMAT_RGB16_565;

	default:
		break;
	}

	return CAIRO_FORMAT_INVALID;
}

static void cairo_xboot_surface_destroy (void * data)
{
	struct cairo_xboot_surface_t * cxs = (struct cairo_xboot_surface_t *) data;

	if (!cxs)
		return;

	free(cxs);
}

cairo_surface_t *
cairo_xboot_surface_create (struct surface_t * surface)
{
	struct cairo_xboot_surface_t * cxs;

	if (!surface)
		return _cairo_surface_create_in_error(_cairo_error(CAIRO_STATUS_NULL_POINTER));

	cxs = malloc(sizeof(struct cairo_xboot_surface_t));
	if (!cxs)
		return _cairo_surface_create_in_error(_cairo_error(CAIRO_STATUS_NO_MEMORY));

	cxs->pixels = surface->pixels;
	cxs->width = surface->w;
	cxs->height = surface->h;
	cxs->fmt = cairo_format_from_pixel_format(surface->info.fmt);

	cxs->surface = surface;
	cxs->cs = cairo_image_surface_create_for_data(cxs->pixels, cxs->fmt, cxs->width,
			cxs->height, cairo_format_stride_for_width(cxs->fmt, surface->w));
	cairo_surface_set_user_data(cxs->cs, NULL, cxs, &cairo_xboot_surface_destroy);

	return cxs->cs;
}
