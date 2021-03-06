/*
 * $Id: AUD_SRCResampleReader.cpp 22328 2009-08-09 23:23:19Z gsrb3d $
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

#include "AUD_SRCResampleReader.h"
#include "AUD_Buffer.h"

#include <math.h>
#include <cstring>
#include <stdio.h>

static long src_callback(void *cb_data, float **data)
{
	return ((AUD_SRCResampleReader*)cb_data)->doCallback(data);
}

AUD_SRCResampleReader::AUD_SRCResampleReader(AUD_IReader* reader,
											 AUD_Specs specs) :
		AUD_EffectReader(reader)
{
	m_sspecs = reader->getSpecs();

	if(m_sspecs.format != AUD_FORMAT_FLOAT32)
	{
		delete m_reader; AUD_DELETE("reader")
		AUD_THROW(AUD_ERROR_READER);
	}

	m_tspecs = specs;
	m_tspecs.channels = m_sspecs.channels;
	m_tspecs.format = m_sspecs.format;
	m_factor = (double)m_tspecs.rate / (double)m_sspecs.rate;

	int error;
	m_src = src_callback_new(src_callback,
							 SRC_SINC_MEDIUM_QUALITY,
							 m_sspecs.channels,
							 &error,
							 this);

	if(!m_src)
	{
		// XXX printf("%s\n", src_strerror(error));
		delete m_reader; AUD_DELETE("reader")
		AUD_THROW(AUD_ERROR_READER);
	}

	m_buffer = new AUD_Buffer(); AUD_NEW("buffer")
}

AUD_SRCResampleReader::~AUD_SRCResampleReader()
{
	delete m_buffer; AUD_DELETE("buffer")

	src_delete(m_src);
}

long AUD_SRCResampleReader::doCallback(float** data)
{
	int length = m_buffer->getSize() / 4 / m_tspecs.channels;
	sample_t* buffer;

	m_reader->read(length, buffer);

	*data = (float*)buffer;
	return length;
}

void AUD_SRCResampleReader::seek(int position)
{
	m_reader->seek(position / m_factor);
	src_reset(m_src);
}

int AUD_SRCResampleReader::getLength()
{
	return m_reader->getLength() * m_factor;
}

int AUD_SRCResampleReader::getPosition()
{
	return m_reader->getPosition() * m_factor;
}

AUD_Specs AUD_SRCResampleReader::getSpecs()
{
	return m_tspecs;
}

void AUD_SRCResampleReader::read(int & length, sample_t* & buffer)
{
	if(m_buffer->getSize() < length * m_tspecs.channels * 4)
		m_buffer->resize(length * m_tspecs.channels * 4);

	buffer = m_buffer->getBuffer();

	length = src_callback_read(m_src, m_factor, length, (float*)buffer);
}
