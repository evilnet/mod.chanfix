-- mod.chanfix SQL Database Script
-- (c) 2003 Matthias Crauwels <ultimate_@wol.be>
-- $Id$

CREATE TABLE chanOps (
	channel VARCHAR(200) NOT NULL,
	account VARCHAR(24) NOT NULL,
	last_seen_as VARCHAR(128),
	ts_firstopped INT4 DEFAULT 0,
	ts_lastopped INT4 DEFAULT 0,
	day0 INT2 NOT NULL DEFAULT 0,
	day1 INT2 NOT NULL DEFAULT 0,
	day2 INT2 NOT NULL DEFAULT 0,
	day3 INT2 NOT NULL DEFAULT 0,
	day4 INT2 NOT NULL DEFAULT 0,
	day5 INT2 NOT NULL DEFAULT 0,
	day6 INT2 NOT NULL DEFAULT 0,
	day7 INT2 NOT NULL DEFAULT 0,
	day8 INT2 NOT NULL DEFAULT 0,
	day9 INT2 NOT NULL DEFAULT 0,
	day10 INT2 NOT NULL DEFAULT 0,
	day11 INT2 NOT NULL DEFAULT 0,
	day12 INT2 NOT NULL DEFAULT 0,
	day13 INT2 NOT NULL DEFAULT 0,
	PRIMARY KEY (channel, account)
);

CREATE TABLE channels (
	channel VARCHAR(200) NOT NULL,
	flags INT4 NOT NULL DEFAULT 0,
	PRIMARY KEY (channel)
);
