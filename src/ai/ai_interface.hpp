/* $Id$ */
/*
   Copyright (C) 2003 - 2009 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2
   or at your option any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file ai/ai_interface.hpp
 * Interface to the AI.
 */

#ifndef AI_AI_INTERFACE_HPP_INCLUDED
#define AI_AI_INTERFACE_HPP_INCLUDED

#include "../global.hpp"

#include "../formula_callable.hpp"

#include "game_info.hpp"

#include "default/contexts.hpp"

namespace ai {

class interface {
public:
	/**
	 * The constructor.
	 */
	interface() {
	}
	virtual ~interface() {}

	/**
	 * Function that is called when the AI must play its turn.
	 * Derived classes should implement their AI algorithm in this function.
	 */
	virtual void play_turn() = 0;

	/**
	 * Function called when a a new turn is played
	 * Derived AIs should call this function each turn (expect first)
	 */
	virtual void new_turn() {
	}

	/**
	 * Function called after the new ai is created
	 *
	 */
	virtual void on_create() {
	}

	virtual void switch_side(ai::side_number side) = 0;

        /** Evaluate */
        virtual std::string evaluate(const std::string& /*str*/)
			{ return "evaluate command not implemented by this AI"; }

	/** Describe self*/
	virtual std::string describe_self() const;

};

class ai_factory;

class ai_factory{
public:
	typedef boost::shared_ptr< ai_factory > factory_ptr;
	typedef std::map<std::string, factory_ptr> factory_map;
	typedef std::pair<const std::string, factory_ptr> factory_map_pair;

	static factory_map& get_list() {
		static factory_map *ai_factories;
		if (ai_factories==NULL) {
			ai_factories = new factory_map;
		}
		return *ai_factories;
	}

	/* cfg is commented out so far, because ai parameter handling is a mess atm */
	virtual ai_ptr get_new_instance( default_ai_context &context/*, const config &cfg*/) = 0;

	ai_factory( const std::string &name )
	{
		factory_ptr ptr_to_this(this);
		get_list().insert(make_pair(name,ptr_to_this));
	}
};


template<class AI>
class register_ai_factory : public ai_factory {
public:
	register_ai_factory( const std::string &name )
		: ai_factory( name )
	{
	}

	virtual ai_ptr get_new_instance( default_ai_context &context/*, const config &cfg*/){
		ai_ptr a(new AI(context));
		a->on_create();
		return a;
	}
};



} //end of namespace ai

#endif
