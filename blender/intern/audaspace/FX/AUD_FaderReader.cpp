/*
 * $Id: AUD_FaderReader.cpp 22328 2009-08-09 23:23:19Z gsrb3d $
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

#include "AUD_FaderReader.h"
#include "AUD_Buffer.h"

#include <cstring>

AUD_FaderReader::AUD_FaderReader(AUD_IReader* reader, AUD_FadeType type,
								 float start,float length) :
		AUD_EffectReader(reader),
		m_type(type),
		m_start(start),
		m_length(length)
{
	int bigendian = 1;
	bigendian = (((char*)&bigendian)[0]) ? 0: 1; // 1 if Big Endian

	switch(m_reader->getSpecs().format)
	{
	case AUD_FORMAT_S16:
		m_adjust = AUD_volume_adjust<int16_t>;
		break;
	case AUD_FORMAT_S32:
		m_adjust = AUD_volume_adjust<int32_t>;
		break;
	case AUD_FORMAT_FLOAT32:
		m_adjust = AUD_volume_adjust<float>;
		break;
	case AUD_FORMAT_FLOAT64:
		m_adjust = AUD_volume_adjust<double>;
		break;
	case AUD_FORMAT_U8:
		m_adjust = AUD_volume_adjust_u8;
		break;
	case AUD_FORMAT_S24:
		m_adjust = bigendian ? AUD_volume_adjust_s24_be :
							   AUD_volume_adjust_s24_le;
		break;
	default:
		delete m_reader;
		AUD_THROW(AUD_ERROR_READER);
	}

	m_buffer = new AUD_Buffer(); AUD_NEW("buffer")
}

AUD_FaderReader::~AUD_FaderReader()
{
	delete m_buffer; AUD_DELETE("buffer")
}

bool AUD_FaderReader::notify(AUD_Message &message)
{
	return m_reader->notify(message);
}

void AUD_FaderReader::read(int & length, sample_t* & buffer)
{
	int position = m_reader->getPosition();
	AUD_Specs specs = m_reader->getSpecs();
	int samplesize = AUD_SAMPLE_SIZE(specs);

	m_reader->read(length, buffer);

	if(m_buffer->getSize() < length * samplesize)
		m_buffer->resize(length * samplesize);

	if((position + length) / (float)specs.rate <= m_start)
	{
		if(m_type != AUD_FADE_OUT)
		{
			buffer = m_buffer->getBuffer();
			memset(buffer,
				   specs.format == AUD_FORMAT_U8 ? 0x80 : 0,
				   length * samplesize);
		}
	}
	else if(position / (float)specs.rate >= m_start+m_length)
	{
		if(m_type == AUD_FADE_OUT)
		{
			buffer = m_buffer->getBuffer();
			memset(buffer,
				   specs.format == AUD_FORMAT_U8 ? 0x80 : 0,
				   length * samplesize);
		}
	}
	else
	{
		sample_t* buf = m_buffer->getBuffer();
		float volume;

		for(int i = 0; i < length; i++)
		{
			volume = (((position+i)/(float)specs.rate)-m_start) / m_length;
			if(volume > 1.0f)
				volume = 1.0f;
			else if(volume < 0.0f)
				volume = 0.0f;

			if(m_type == AUD_FADE_OUT)
				volume = 1.0f - volume;

			m_adjust(buf + i * samplesize, buffer + i * samplesize,
					 specs.channels, volume);
		}

		buffer = buf;
	}
}
