CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration

INTRODUCTION
------------
This module allows you to specify custom node and page templates on a per-node
basis.
 * For a full description of the module, visit the sandbox page:
   https://www.drupal.org/sandbox/s-robertson/2341113
 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2341113?categories=All

REQUIREMENTS
------------
This module has no requirements, aside from the core Node module.

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * You likely want to disable Toolbar module, since its output clashes with
   Administration menu.

CONFIGURATION
-------------
 * Configure user permissions in Administration � People � Permissions:
   - Use custom template suggestions
     Top-level administrators require this permission to be able to configure
     custom templates for nodes.
 * Configure template suggestions in the "Custom template suggestions" section
   of the vertical tabs when adding/editing a node.
