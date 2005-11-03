/**
 * sqlManager.cc
 *
 * Author: Rasmus Hansen <jeekay@netgamers.org>
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

#include <new>
#include <iostream>
#include <boost/shared_ptr.hpp>
	
#include <cassert>

#include "gnuworld_config.h"
#include "ELog.h"
#include "gThread.h"
#include "sqlManager.h"

namespace gnuworld {

/* Initialise our static data members */
sqlManager* sqlManager::theManager = 0;

/*********************************
 ** S T A T I C   M E T H O D S **
 *********************************/

/**
 * Implementation of singleton - this is the only way to get a reference to
 * the sqlManager instance, and it does not allow modification.
 * initialise must be called prior to attempted to obtain an instance.
 * This method is static.
 */
sqlManager* sqlManager::getInstance(const std::string& _dbString, int _commitQueueMax)
{
if(theManager) return theManager;

/* There is currently no sqlManager instance */
return new sqlManager(_dbString, _commitQueueMax);
} // static sqlManager* sqlManager::getInstance(const std::string&)



/*********************************
 ** P U B L I C   M E T H O D S **
 *********************************/

/**
 * This method creates and returns a connection to the database.
 * It will use the stored dbString to create the connection.
 */
PgDatabase* sqlManager::getConnection()
{
elog << "*** [sqlManager:getConnection] Attempting DB connection to: "
  << dbString << std::endl;

PgDatabase* tempCon = new (std::nothrow) PgDatabase(dbString.c_str());
assert(tempCon != 0);

if(tempCon->ConnectionBad()) {
  elog << "*** [sqlManager:getConnection] Unable to connect to DB: "
    << tempCon->ErrorMessage() << std::endl;
  ::exit(0);
} else {
  elog << "*** [sqlManager:getConnection] Connection established to DB."
    << std::endl;
}

return tempCon;
}


/**
 * This method will disconnect and destroy a database connection.
 * This method should never be passed a null pointer.
 */
void sqlManager::removeConnection(PgDatabase* tempCon)
{
assert(tempCon != 0);

elog << "*** [sqlManager:removeConnection] Removing DB connection." << std::endl;
delete tempCon;
}

class SQLQueue : public gThread
{
public:
        SQLQueue(sqlManager& sq) : sq_(sq) {}

        virtual void Exec()
        {
		for(sqlManager::CommitQueueItr ptr = sq_.commitQueue.begin(); ptr != sq_.commitQueue.end(); ++ptr) {
			std::string statement = *ptr;

#ifdef LOG_SQL
			elog << "*** [sqlManager:flush] Executing: " << statement << std::endl;
#endif
			if(!sq_.SQLDb->ExecCommandOk(statement.c_str())) {
				std::string error = std::string(sq_.SQLDb->ErrorMessage());

#ifndef LOG_SQL
				/* Make sure people without LOG_SQL still see what statement failed */
				elog << "*** [sqlManager:flush] Executing: " << statement << std::endl;
#endif
				elog << "*** [sqlManager:flush] Error: " << error << std::endl;
				// TODO: Log error
			}
		}
		sq_.commitQueue.clear();
        }

private:
        sqlManager& sq_;
};

boost::shared_ptr<SQLQueue> sqs;


/**
 * This method simply processes all statements in the queue, executing
 * them against the database.
 * TODO: Should this be inside a transaction?
 */
void sqlManager::flush()
{
        sqs = boost::shared_ptr<SQLQueue>(new SQLQueue(*this));
        sqs->Start();
}


/**
 * This method allows a caller to add a statement to the commit queue.
 * The statement will be executed against the database when the next commit
 * interval occurs.
 */
void sqlManager::queueCommit(const std::string& theStatement)
{
  commitQueue.push_back(theStatement);

  if(commitQueue.size() >= commitQueueMax) {
    flush();
  }
}


/*****************************************************
 ** C O N S T R U C T O R   &   D E S T R U C T O R **
 *****************************************************/

/**
 * This is our constructor that initialises DB communications
 * and any of the queues that will be used
 * It is only ever called from initialise()
 */
sqlManager::sqlManager(const std::string& _dbString, int _commitQueueMax)
{
/* Construct our DB object and initialise queues */
dbString = _dbString;
SQLDb = getConnection();
commitQueueMax = _commitQueueMax;
} // sqlManager::sqlManager


/**
 * This is our constructor that ensures the proper teardown of
 * our DB link and clears out any memory currently in use by
 * queues or objects
 */
sqlManager::~sqlManager()
{
/* Destruct our DB object and any queues */
elog << "*** [sqlManager] Shutting down DB communications." << std::endl;
removeConnection(SQLDb);
} // sqlManager::~sqlManager()


} // namespace gnuworld
