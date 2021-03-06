/*
 * $Id: AUD_FileFactory.cpp 22687 2009-08-21 22:06:19Z gsrb3d $
 *
 * ***** BEGIN LGPL LICENSE BLOCK *****
 *
 * Copyright 2009 Jörg Hermann Müller
 *
 * This file is part of AudaSpace.
 *
 * AudaSpace is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AudaSpace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AudaSpace.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ***** END LGPL LICENSE BLOCK *****
 */

#include "AUD_FileFactory.h"
#include "AUD_Buffer.h"

#include <cstring>

#ifdef WITH_FFMPEG
#include "AUD_FFMPEGReader.h"
#endif
#ifdef WITH_SNDFILE
#include "AUD_SndFileReader.h"
#endif

AUD_FileFactory::AUD_FileFactory(const char* filename)
{
	if(filename != NULL)
	{
		m_filename = new char[strlen(filename)+1]; AUD_NEW("string")
		strcpy(m_filename, filename);
	}
	else
		m_filename = NULL;
}

AUD_FileFactory::AUD_FileFactory(unsigned char* buffer, int size)
{
	m_filename = NULL;
	m_buffer = AUD_Reference<AUD_Buffer>(new AUD_Buffer(size));
	memcpy(m_buffer.get()->getBuffer(), buffer, size);
}

AUD_FileFactory::~AUD_FileFactory()
{
	if(m_filename)
	{
		delete[] m_filename; AUD_DELETE("string")
	}
}

AUD_IReader* AUD_FileFactory::createReader()
{
	AUD_IReader* reader = 0;

#ifdef WITH_SNDFILE
	try
	{
		if(m_filename)
			reader = new AUD_SndFileReader(m_filename);
		else
			reader = new AUD_SndFileReader(m_buffer);
		AUD_NEW("reader")
		return reader;
	}
	catch(AUD_Exception e) {}
#endif

#ifdef WITH_FFMPEG
	try
	{
		if(m_filename)
			reader = new AUD_FFMPEGReader(m_filename);
		else
			reader = new AUD_FFMPEGReader(m_buffer);
		AUD_NEW("reader")
		return reader;
	}
	catch(AUD_Exception e) {}
#endif

	return reader;
}
