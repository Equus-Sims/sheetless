<?php
/**
 * @file
 * equus_footer_menu.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function equus_footer_menu_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-account.
  $menus['menu-account'] = array(
    'menu_name' => 'menu-account',
    'title' => 'Account',
    'description' => 'Account menu for footer',
  );
  // Exported menu: menu-footer-menu.
  $menus['menu-footer-menu'] = array(
    'menu_name' => 'menu-footer-menu',
    'title' => 'Directories',
    'description' => 'Directories menu in footer',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Account');
  t('Account menu for footer');
  t('Directories');
  t('Directories menu in footer');


  return $menus;
}
