/**
 * UNSUSPENDCommand.cc
 *
 * 08/29/2005 - Reed Loden <reed@reedloden.com>
 * Initial Version
 *
 * Unsuspends a user
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 *
 * $Id$
 */

#include "gnuworld_config.h"

#include "chanfix.h"
#include "responses.h"
#include "StringTokenizer.h"
#include "sqlUser.h"

RCSTAG("$Id$");

namespace gnuworld
{

void UNSUSPENDCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
{
StringTokenizer st(Message);

sqlUser* targetUser = bot->isAuthed(st[1]);
if (!targetUser) {
  bot->SendTo(theClient,
	      bot->getResponse(theUser,
			language::no_such_user,
			std::string("No such user %s.")).c_str(), st[1].c_str());
  return;
}

/* Can't unsuspend an owner unless you're an owner. */
if (targetUser->getFlag(sqlUser::F_OWNER) && !theUser->getFlag(sqlUser::F_OWNER)) {
  bot->SendTo(theClient,
	      bot->getResponse(theUser,
			language::cant_suspend_an_owner,
			std::string("You cannot unsuspend an owner unless you're an owner.")).c_str());
  return;
}

/* Can only unsuspend a user manager if you're an owner. */
if (targetUser->getFlag(sqlUser::F_USERMANAGER) && !theUser->getFlag(sqlUser::F_OWNER)) {
  bot->SendTo(theClient,
	      bot->getResponse(theUser,
			language::cant_unsuspend_manager,
			std::string("You cannot unsuspend a user manager unless you're an owner.")).c_str());
  return;
}

/* A serveradmin can only unsuspend users in his/her own group. */
if (theUser->getFlag(sqlUser::F_SERVERADMIN) &&
    !theUser->getFlag(sqlUser::F_USERMANAGER)) {
  if (targetUser->getGroup() != theUser->getGroup()) {
    bot->SendTo(theClient,
		bot->getResponse(theUser,
			language::cant_unsuspend_diff_group,
			std::string("You cannot unsuspend a user in a different group.")).c_str());
    return;
  }
}

if (!targetUser->getIsSuspended()) {
  bot->SendTo(theClient,
	      bot->getResponse(theUser,
			language::user_not_suspended,
			std::string("User %s is not currently suspended.")).c_str(),
				targetUser->getUserName().c_str());
  return;
}

targetUser->setSuspended(false);
targetUser->setLastUpdated(bot->currentTime());
targetUser->setLastUpdatedBy( std::string( "("
	+ theUser->getUserName()
	+ ") "
	+ theClient->getRealNickUserHost() ) );
targetUser->commit();

bot->SendTo(theClient,
	    bot->getResponse(theUser,
			language::user_unsuspended,
			std::string("Unsuspended user %s.")).c_str(),
				targetUser->getUserName().c_str());
bot->logAdminMessage("%s (%s) unsuspended user %s.",
	    theUser->getUserName().c_str(),
	    theClient->getRealNickUserHost().c_str(),
	    targetUser->getUserName().c_str());

return;
} //UNSUSPENDCommand::Exec
} //Namespace gnuworld
