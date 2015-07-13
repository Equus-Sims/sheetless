The UserDashboard module forks Drupal 7's core Dashboard module to provide an
individual dashboard for each user on the site.

Users can access their dashboards at the /user/dashboard
(or /user/[uid]/dashboard) page, and take advantage of the same drag & drop
functionality as in the original Dashboard module.

Administrators can configure which blocks can be used on the user dashboard
via the settings form at /admin/dashboard/user_dashboard/settings.

Users with the 'set default user_dashboard blocks' permission will also see
a 'set blocks as default' button on their customize dashboard page, which will
set their current block layout as the default for new users.