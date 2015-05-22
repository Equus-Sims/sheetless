<?php
/**
 * @file
 * equus_footer_menu.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function equus_footer_menu_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-account_dashboard:node/177
  $menu_links['menu-account_dashboard:node/177'] = array(
    'menu_name' => 'menu-account',
    'link_path' => 'node/177',
    'router_path' => 'node/%',
    'link_title' => 'Dashboard',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-account_dashboard:node/177',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-account_logout:user/logout
  $menu_links['menu-account_logout:user/logout'] = array(
    'menu_name' => 'menu-account',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Logout',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-account_logout:user/logout',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu_horses:horse-directory
  $menu_links['menu-footer-menu_horses:horse-directory'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'horse-directory',
    'router_path' => 'horse-directory',
    'link_title' => 'Horses',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-menu_horses:horse-directory',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu_members:member-directory
  $menu_links['menu-footer-menu_members:member-directory'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'member-directory',
    'router_path' => 'member-directory',
    'link_title' => 'Members',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-menu_members:member-directory',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 1,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard');
  t('Horses');
  t('Logout');
  t('Members');


  return $menu_links;
}
