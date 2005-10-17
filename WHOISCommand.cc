/**
 * WHOISCommand.cc
 *
 * 08/07/2005 - Jimmy Lipham <music0m@alltel.net>
 * Initial Version
 *
 * Shows information about this user
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

void WHOISCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
{
StringTokenizer st(Message);

if (st[1] == "*") {
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::list_of_all_users,
                              std::string("List of all users:")).c_str());
  chanfix::usersIterator ptr = bot->usersMap_begin();
  while (ptr != bot->usersMap_end()) {
    sqlUser* tmpUser = ptr->second;
    bot->SendTo(theClient,
                bot->getResponse(theUser,
                                language::user_flags_group,
                                std::string("User: %s, Flags: %s, Group: %s")).c_str(),
                                            tmpUser->getUserName().c_str(), (tmpUser->getFlags()) ?
                                            std::string("+" + bot->getFlagsString(tmpUser->getFlags())).c_str() : "None", 
                                            tmpUser->getGroup().c_str());
    ptr++;
  }
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::end_of_user_list,
                              std::string("End of user list.")).c_str());
  return;
}

sqlUser* theUser2 = bot->isAuthed(st[1]);
if (!theUser2) 
{ 
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::no_such_user,
                              std::string("No such user %s.")).c_str(),st[1].c_str());
  return;
}

bot->SendTo(theClient,
            bot->getResponse(theUser,
                            language::whois_user,
                            std::string("User: %s")).c_str(), theUser2->getUserName().c_str());
if (!theUser2->getFlags())
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::whois_flags_none,
                              std::string("Flags: none.")).c_str());
else
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::whois_flags,
                              std::string("Flags: +%s")).c_str(),
                                          bot->getFlagsString(theUser2->getFlags()).c_str());

sqlUser::hostListType sqlHostList = theUser2->getHostList();
std::stringstream hostlist;

if (sqlHostList.size() > 0) {
  for(sqlUser::hostListType::iterator itr = sqlHostList.begin() ;
    itr != sqlHostList.end() ; ++itr) {
      if (hostlist.str() == "")
        hostlist << *itr;
      else
	hostlist << ", " << *itr;
    }
}
if (hostlist.str() == "") hostlist << "None.";
bot->SendTo(theClient,
            bot->getResponse(theUser,
                            language::whois_hosts,
                            std::string("Hosts: %s")).c_str(), hostlist.str().c_str());
bot->SendTo(theClient,
            bot->getResponse(theUser,
                            language::whois_group,
                            std::string("Group: %s")).c_str(), theUser2->getGroup().c_str());

return;
} //whoiscommand::exec
} //namespace gnuworld
