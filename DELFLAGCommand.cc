/**
 * DELFLAGCommand.cc
 *
 * 08/08/2005 - Jimmy Lipham <music0m@alltel.net>
 * Initial Version
 *
 * Removes this flag from the user
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
#include "StringTokenizer.h"
#include "sqlUser.h"

RCSTAG("$Id$");

namespace gnuworld
{

void DELFLAGCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
{
StringTokenizer st(Message);

char flag = st[2][0];
if (st[2].size() > 1) {
  if (flag == '-')
    flag = st[2][1];
  else {
    bot->Notice(theClient, "You may only remove one flag per DELFLAG command.");
    return;
  }
}

if (!bot->getFlagType(flag)) {
  Usage(theClient);
  return;
}

sqlUser* chkUser = bot->isAuthed(st[1]);
if (!chkUser) {
  bot->Notice(theClient, "No such user %s.", st[1].c_str());
  return;
}

if (flag == bot->getFlagChar(sqlUser::F_OWNER)) {
  bot->Notice(theClient, "You cannot delete an owner flag.");
  return;
}

if (flag == bot->getFlagChar(sqlUser::F_USERMANAGER) &&
    !theUser->getFlag(sqlUser::F_OWNER)) {
  bot->Notice(theClient, "Only an owner can delete the user management flag.");
  return;
}

/* A serveradmin can only delete flags from users on his/her own server. */
if (theUser->getFlag(sqlUser::F_SERVERADMIN) &&
    !theUser->getFlag(sqlUser::F_USERMANAGER)) {
//  if (chkUser->getMainGroup() != theUser->getMainGroup()) {
//    bot->Notice(theClient, "You cannot delete a flag from a user with a different main group.");
//    return;
//  }
  if (flag == bot->getFlagChar(sqlUser::F_BLOCK)) {
    bot->Notice(theClient, "You cannot remove a block flag.");
    return;
  }
  if (flag == bot->getFlagChar(sqlUser::F_SERVERADMIN)) {
    bot->Notice(theClient, "You cannot remove a serveradmin flag.");
    return;
  }
}

if (!chkUser->getFlag(bot->getFlagType(flag))) {
  bot->Notice(theClient, "User %s does not have flag %c.",
	      chkUser->getUserName().c_str(), flag);
  return;
}

chkUser->removeFlag(flag);
chkUser->setLastUpdatedBy(theUser->getUserName());
chkUser->setLastUpdated(bot->currentTime());
chkUser->commit();
bot->Notice(theClient, "Deleted flag %c of user %s.", flag,
	    chkUser->getUserName().c_str());
} //DELFLAGCommand::Exec
} //Namespace gnuworld
