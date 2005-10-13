/**
 * ADDHOSTCommand.cc
 *
 * 08/26/2005 - Jimmy Lipham <music0m@alltel.net>
 * Initial Version
 *
 * Adds this hostmask to the user's list of hostmasks
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

void ADDHOSTCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
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

/* A serveradmin can only add flags to users on his/her own group. */
if (theUser->getFlag(sqlUser::F_SERVERADMIN) &&
    !theUser->getFlag(sqlUser::F_USERMANAGER)) {
  if (targetUser->getGroup() != theUser->getGroup()) {
    bot->SendTo(theClient,
                bot->getResponse(theUser,
                                language::cant_add_host_diff_group,
                                std::string("You cannot add a host to a user in a different group.")).c_str());
    return;
  }
}
  
if (targetUser->matchHost(st[2].c_str())) {
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::already_has_hostmask,
                              std::string("User %s already has hostmask %s.")).c_str(),
                                          targetUser->getUserName().c_str(), st[2].c_str());
  return;
}

std::stringstream insertString;
insertString	<< "INSERT INTO hosts "
		<< "(user_id, host) VALUES "
		<< "("
		<< targetUser->getID()
		<< ", '"
		<< st[2].c_str()
		<< "')"
		;

ExecStatusType status = bot->SQLDb->Exec(insertString.str().c_str());

if (PGRES_COMMAND_OK != status) {
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::failed_adding_hostmask,
                              std::string("Failed adding hostmask %s to user %s.")).c_str(),
                                          st[2].c_str(), targetUser->getUserName().c_str());
  return;
}
  
targetUser->addHost(st[2].c_str());
targetUser->setLastUpdated(bot->currentTime());
targetUser->setLastUpdatedBy( std::string( "("
	+ theUser->getUserName()
	+ ") "
	+ theClient->getRealNickUserHost() ) );
targetUser->commit();

bot->SendTo(theClient,
            bot->getResponse(theUser,
                            language::adding_hostmask_to_user,
                            std::string("Added hostmask %s to user %s.")).c_str(),
                                        targetUser->getUserName().c_str());
bot->logAdminMessage("%s (%s) added hostmask %s to user %s.",
		     theUser->getUserName().c_str(), 
		     theClient->getRealNickUserHost().c_str(), 
		     st[2].c_str(), targetUser->getUserName().c_str());

return;
}

} //namespace gnuworld
