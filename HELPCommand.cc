/**
 * HELPCommand.cc
 *
 * 08/08/2005 - Jimmy Lipham <music0m@alltel.net>
 * Initial Version
 *
 * Shows help about <command> or gives a general list of commands
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

void HELPCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
{
StringTokenizer st(Message);

if (st.size() < 2) {
  bot->Notice(theClient, "\002Oper Level\002: SCORE CSCORE HISTORY INFO STATUS");

  if (theClient->getAccount() != "")
    bot->Notice(theClient, "\002Logged In\002: CHECK WHO");

  if (!theUser) return;

  if (theUser->getFlag(sqlUser::F_SERVERADMIN))
    bot->Notice(theClient, "\002Server Admin (+a)\002: ADDUSER DELUSER ADDHOST DELHOST ADDFLAG DELFLAG WHOSERVER");

  if (theUser->getFlag(sqlUser::F_BLOCK))
    bot->Notice(theClient, "\002Blocker (+b)\002: BLOCK UNBLOCK");

  if (theUser->getFlag(sqlUser::F_CHANNEL))
    bot->Notice(theClient, "\002Channel (+c)\002: ADDNOTE DELNOTE ALERT UNALERT");

  if (theUser->getFlag(sqlUser::F_CHANFIX))
    bot->Notice(theClient, "\002Chanfixer (+f)\002: CHANFIX OPLIST OPNICKS");

  if (theUser->getFlag(sqlUser::F_OWNER))
    bot->Notice(theClient, "\002Owner (+o)\002: QUOTE REHASH RELOAD SET SHUTDOWN");

  if (theUser->getFlag(sqlUser::F_USERMANAGER))
    bot->Notice(theClient, "\002User Manager (+u)\002: WHOIS ADDUSER DELUSER ADDHOST DELHOST ADDSERVER DELSERVER ADDFLAG DELFLAG WHOSERVER");

} else {
  bot->Notice(theClient, "No help available on that topic.");
}

} //helpcommand::exec
} //namespace gnuworld
