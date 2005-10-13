/**
 * sqlUser.h
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 *
 * $Id$
 */

#ifndef __SQLUSER_H
#define __SQLUSER_H "$Id$"

#include	<string>
#include	<vector>
#include	<ctime>
#include	"libpq++.h"

namespace gnuworld
{

class iClient;

class sqlUser
{

public:

	sqlUser(PgDatabase*);
	virtual ~sqlUser();

	typedef unsigned short int	flagType ;
	static const flagType F_SERVERADMIN; /* +a */
	static const flagType F_BLOCK; /* +b */
	static const flagType F_CHANNEL; /* +c */
	static const flagType F_CHANFIX; /* +f */
	static const flagType F_OWNER; /* +o */
	static const flagType F_USERMANAGER; /* +u */
	static const flagType F_LOGGEDIN;

	typedef std::list< std::string >	hostListType;

	/* Methods to get data attributes. */

	inline const unsigned int&	getID() const
		{ return id ; }

	inline const std::string& getUserName() const
		{ return user_name; }

	inline const int getCreated() const
		{ return created; }

	inline const unsigned int getLastSeen() const
		{ return last_seen; }

	inline const unsigned int getLastUpdated() const
		{ return last_updated; }

	inline const std::string& getLastUpdatedBy() const
		{ return last_updated_by; }

	inline const unsigned int&	getLanguageId() const
		{ return language_id ; }

	inline const std::string& getGroup() const
		{ return group; }

	inline bool	getFlag( const flagType& whichFlag ) const
		{ return ((flags & F_OWNER) || (flags & whichFlag)) ; }

	inline const flagType&		getFlags() const
		{ return flags ; }

	inline const bool getIsSuspended() const
		{ return isSuspended; }

	inline const bool getUseNotice() const
		{ return useNotice; }
		
	inline const hostListType& getHostList() const
		{ return hostList; }

	/* Mutators */

	inline void setUserName(const std::string& _user_name)
		{ user_name = _user_name; }

	inline void setCreated(const unsigned int _created)
		{ created = _created; }

	inline void setLastSeen(const unsigned int _last_seen)
		{ last_seen = _last_seen; }

	inline void setLastUpdated(const unsigned int _last_updated)
		{ last_updated = _last_updated; }

	inline void setLastUpdatedBy(const std::string& _last_updated_by)
		{ last_updated_by = _last_updated_by; }

	inline void setLanguageId(const unsigned int _language_id)
		{ language_id = _language_id; }

	inline void setGroup(const std::string& _group)
		{ group = string_lower(_group); }

	inline void setFlag(const flagType& whichFlag)
		{ flags |= whichFlag; }

	inline void removeFlag(const flagType& whichFlag)
		{ flags &= ~whichFlag; }

	inline void setSuspended(const bool _isSuspended)
		{ isSuspended = _isSuspended; }

	inline void setNotice(const bool _useNotice)
		{ useNotice = _useNotice; }

	inline void addHost(const std::string& _theHost)
		{ hostList.push_back(_theHost); }

	/* Methods to alter our SQL status */
	bool loadData(unsigned int);
	bool loadData(const std::string&);
	void setAllMembers(int);
	bool commit();
	bool Insert();
	bool Delete();

	void loadHostList();
	void delHost(const std::string&);		
	bool matchHost(const std::string&);
	bool hasHost(const std::string&);

private:

	unsigned int	id;
	std::string	user_name;
	unsigned int	created;
	unsigned int	last_seen;
	unsigned int	last_updated;
	std::string	last_updated_by;
	unsigned int	language_id;
	std::string	group;
	flagType	flags;
	bool		isSuspended;
	bool		useNotice;
	hostListType	hostList;

	PgDatabase*	SQLDb;
}; // class sqlUser

} // namespace gnuworld

#endif // __SQLUSER_H
