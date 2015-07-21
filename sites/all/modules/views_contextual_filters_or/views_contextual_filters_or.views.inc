<?php

/**
 * @file
 * Primarily Drupal hooks to manipulate Views Contextual Filters OR.
 *
 * This is the main module file for Views Contextual Filters OR.
 */


/**
 * Implements hook_views_plugins_alter().
 */
function views_contextual_filters_or_views_plugins_alter(&$plugins) {
  $plugins['query']['views_query']['handler'] = 'views_contextual_filters_or_query';
}
