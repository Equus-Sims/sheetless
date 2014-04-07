<?php

/**
 * Access plugin that provides permission-based access control.
 */
class views_plugin_access_user_arg1 extends views_plugin_access {

  function access($account) {
    return views_plugin_access_user_arg_check($account);
  }

  function get_access_callback() {
    return array('views_plugin_access_user_arg_check');
  }

  function summary_title() {
    return t('Arg 1 user & transaction manager');
  }
}
