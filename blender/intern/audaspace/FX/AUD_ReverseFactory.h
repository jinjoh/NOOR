/*
 * $Id: AUD_ReverseFactory.h 22328 2009-08-09 23:23:19Z gsrb3d $
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

#ifndef AUD_REVERSEFACTORY
#define AUD_REVERSEFACTORY

#include "AUD_EffectFactory.h"

/**
 * This factory reads another factory reverted.
 * \note Readers from the underlying factory must be from the buffer type.
 */
class AUD_ReverseFactory : public AUD_EffectFactory
{
public:
	/**
	 * Creates a new reverse factory.
	 * \param factory The input factory.
	 */
	AUD_ReverseFactory(AUD_IFactory* factory = 0);

	/**
	 * Destroys the factory.
	 */
	virtual AUD_IReader* createReader();
};

#endif //AUD_REVERSEFACTORY
