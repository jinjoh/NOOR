/*
 * $Id: AUD_EffectFactory.cpp 22328 2009-08-09 23:23:19Z gsrb3d $
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

#include "AUD_EffectFactory.h"
#include "AUD_IReader.h"

AUD_IReader* AUD_EffectFactory::getReader()
{
	if(m_factory != 0)
		return m_factory->createReader();

	return 0;
}

AUD_EffectFactory::AUD_EffectFactory(AUD_IFactory* factory)
{
	m_factory = factory;
}

void AUD_EffectFactory::setFactory(AUD_IFactory* factory)
{
	m_factory = factory;
}

AUD_IFactory* AUD_EffectFactory::getFactory()
{
	return m_factory;
}