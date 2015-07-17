<?php

/**
 * @file control_moderation.views_default.inc
 * Default moderation views for the Total Control Admin Dashboard.
 */

/**
 * Implements hook_views_default_views().
 */
function total_control_moderation_views_default_views() {
  $views_path = drupal_get_path('module', 'total_control_moderation') . '/views';
  $files = file_scan_directory($views_path, '/\.view$/');

  foreach ($files as $filepath => $file) {
    require $filepath;

    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }

  return $views;
}