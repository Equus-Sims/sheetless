<?php

/*
 * User chooser module
 * Defines 'segments' of users with a hook
 * provides form widgets, validation and submission
 * provides function to determine if a user is in a segment
*/


/*
 * implements hook_uchoo_segments
 * returns an array suitable for the select widget, so it can be nested. This example here is fictional
 * each of these is a callback which takes an arg and returns an array of uids
 * and another callback 'in_'.CALLBACK which takes an arg and a uid and returns TRUE or FALSE
 */
function user_chooser_uchoo_segments() {
  //segment defines all active users
  $callbacks = array(
    t('User mail hosts') => array(//this is used for grouping in the select widget
      'user_chooser_segment_gmail' => 'All gmail users on the site',
    )
  );
  return $callbacks;
}

/*
 * Example callback
 * select active users
 * Args:
 * $query, a partially built query to the users table: select uid from users
 * $args, as passed from the form element #args
 * $string may be passed from the autocomplete widget
 * $limit - restrict results to this many
*/
function user_chooser_segment_gmail($query, $args = array(), $string = '', $limit = FALSE) {
  $query->condition('mail', '%gmail.com', 'LIKE');
}

/*
 * Example required 'in_' version of callback
 * return whether the user is in the segment
 * this is a different function for performance reasons
 */
function in_user_chooser_segment_gmail($args, $uid) {
  return strpos(user_load($uid)->mail, '.gmail.com');
}

/*
 * example form element, showing defaults where appropriate
 */
function hook_form() {
  $form['fieldname'] = array(
    '#title' => t('Blah blah'),
    '#description' => 'if not declared some helptext will be put here',
    '#type' => 'user_chooser_many',// or user_chooser_many
    '#exclude' => array(1), //these uids will be excluded from the segment
    '#allow_blocked' => FALSE, //refers to $user->status,
    '#multiple_fail_alert' => 1,//how to respond when one uid of several fails validation: values 0 no alert, 1 warning, 2 error
  );
  $form['fieldname'] = array(
    '#title' => t('Blah blah'),
    '#type' => 'user_chooser_few',// or user_chooser_many
    '#callback' => 'user_chooser_segment_gmail', //anything returned from hook_user_chooser_segments
    '#args' => array(), //array to be passed as first arg to the callback function
    '#multiple' => TRUE, //same as in select element
    '#exclude' => array(1), //these uids will be excluded from the segment
    '#allow_blocked' => FALSE, //refers to $user->status,
    '#multiple_fail_alert' => 1,//how to respond when one uid of several fails validation: values 0 no alert, 1 warning, 2 error
  );
  return $form;
}

/*
 * Example to get a segment of users
 * $callback - one of the callbacks in hook_user_chooser_segments
 * $args - an array of args to pass to the $callback
 * $settings an optional array with two optional keys from the element type definition
 * - #allow_blocked
 * - #exclude
 */

user_chooser_segment($callback, $arg, $settings);

/*
 * Example to check if a user is in a segment
 */
in_user_chooser_segment($callback, $arg, $account);