<?php

/**
 * hook_accountmenu_name_realname_alter
 *
 * Alter the list of tokens used to generate the user/login text.
 */
function hook_accountment_name_realname_alter(&$list) {
  $list['@realname'] .= ', the King of Freedonia';
  $list['@my_custom_token'] = 'Additional tokens are possible, too.';
}