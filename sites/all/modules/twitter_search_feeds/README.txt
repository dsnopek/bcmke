/*$Id: README.txt,v 1.1.2.1 2009/06/28 23:49:34 prajwala Exp $ */

SUMMARY
=======
This module reads the twitter feeds based on the search term provided by the site admin and stores all these tweets as nodes. Users of the application can comment it, rate it, can do any thing with these nodes.

REQUIREMENTS
============
Activity Stream
Views

INSTALLATION
============
Install activity stream module
Install twitter search feeds module

USAGE
=====
Provide your seach term at /admin/settings/activitystream path and save.
Run the cron manually to get the results immediately.
If you enable views then you will see twitter_search_feed view, enable it then you will see Twitter Search Feeds navigation link for the twitter feeds based on the search term.