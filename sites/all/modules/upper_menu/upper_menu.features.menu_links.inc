<?php
/**
 * @file
 * upper_menu.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function upper_menu_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_about:node/172
  $menu_links['main-menu_about:node/172'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/172',
    'router_path' => 'node/%',
    'link_title' => 'About',
    'options' => array(
      'identifier' => 'main-menu_about:node/172',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: main-menu_blogs:blogs
  $menu_links['main-menu_blogs:blogs'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blogs',
    'router_path' => 'blogs',
    'link_title' => 'Blogs',
    'options' => array(
      'identifier' => 'main-menu_blogs:blogs',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: main-menu_events:node/175
  $menu_links['main-menu_events:node/175'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/175',
    'router_path' => 'node/%',
    'link_title' => 'Events',
    'options' => array(
      'identifier' => 'main-menu_events:node/175',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: main-menu_forum:node/176
  $menu_links['main-menu_forum:node/176'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/176',
    'router_path' => 'node/%',
    'link_title' => 'Forum',
    'options' => array(
      'identifier' => 'main-menu_forum:node/176',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
  );
  // Exported menu link: main-menu_news:node/173
  $menu_links['main-menu_news:node/173'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/173',
    'router_path' => 'node/%',
    'link_title' => 'News',
    'options' => array(
      'identifier' => 'main-menu_news:node/173',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: user-menu_dashboard:node/177
  $menu_links['user-menu_dashboard:node/177'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'node/177',
    'router_path' => 'node/%',
    'link_title' => 'Dashboard',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'user-menu_dashboard:node/177',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'user-menu_janelle-forsyth:user/self',
  );
  // Exported menu link: user-menu_edit-profile:menutoken/554ab83069553
  $menu_links['user-menu_edit-profile:menutoken/554ab83069553'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'menutoken/554ab83069553',
    'router_path' => 'menutoken/%',
    'link_title' => 'Edit Profile',
    'options' => array(
      'menu_token_link_path' => 'user/[current-user:uid]/edit',
      'menu_token_data' => array(
        'user' => array(
          'type' => 'user',
          'plugin' => 'user_context',
          'options' => array(),
        ),
      ),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'user-menu_edit-profile:menutoken/554ab83069553',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'user-menu_janelle-forsyth:user/self',
  );
  // Exported menu link: user-menu_log-out:user/logout
  $menu_links['user-menu_log-out:user/logout'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Log out',
    'options' => array(
      'identifier' => 'user-menu_log-out:user/logout',
    ),
    'module' => 'system',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 10,
    'customized' => 1,
  );
  // Exported menu link: user-menu_login:user/login
  $menu_links['user-menu_login:user/login'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Login',
    'options' => array(
      'alter' => TRUE,
      'attributes' => array(
        'title' => 'Login to the site.',
        'class' => 'popups-form-reload',
      ),
      'identifier' => 'user-menu_login:user/login',
    ),
    'module' => 'accountmenu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: user-menu_logout:user/logout
  $menu_links['user-menu_logout:user/logout'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Logout',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'user-menu_logout:user/logout',
    ),
    'module' => 'accountmenu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'parent_identifier' => 'user-menu_janelle-forsyth:user/self',
  );
  // Exported menu link: user-menu_messages:messages
  $menu_links['user-menu_messages:messages'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'messages',
    'router_path' => 'messages',
    'link_title' => 'Messages',
    'options' => array(
      'identifier' => 'user-menu_messages:messages',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
  );
  // Exported menu link: user-menu_my-account:user/self
  $menu_links['user-menu_my-account:user/self'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/self',
    'router_path' => 'user/self',
    'link_title' => 'My Account',
    'options' => array(
      'attributes' => array(
        'title' => 'The user\'s own account page.',
      ),
      'identifier' => 'user-menu_my-account:user/self',
    ),
    'module' => 'system',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'user-menu_my-account:user',
  );
  // Exported menu link: user-menu_my-points:myuserpoints
  $menu_links['user-menu_my-points:myuserpoints'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'myuserpoints',
    'router_path' => 'myuserpoints',
    'link_title' => 'My !points',
    'options' => array(
      'identifier' => 'user-menu_my-points:myuserpoints',
    ),
    'module' => 'system',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: user-menu_register:user/register
  $menu_links['user-menu_register:user/register'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/register',
    'router_path' => 'user/register',
    'link_title' => 'Register',
    'options' => array(
      'alter' => TRUE,
      'attributes' => array(
        'title' => 'Register a new account',
        'class' => 'popups-form-reload',
      ),
      'identifier' => 'user-menu_register:user/register',
    ),
    'module' => 'accountmenu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: user-menu_user-account:user
  $menu_links['user-menu_user-account:user'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user',
    'router_path' => 'user',
    'link_title' => 'User account',
    'options' => array(
      'alter' => TRUE,
      'identifier' => 'user-menu_user-account:user',
    ),
    'module' => 'system',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -10,
    'customized' => 1,
  );
  // Exported menu link: user-menu_view-profile:user/self
  $menu_links['user-menu_view-profile:user/self'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/self',
    'router_path' => 'user/self',
    'link_title' => 'View Profile',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'user-menu_view-profile:user/self',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'user-menu_janelle-forsyth:user/self',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('About');
  t('Blogs');
  t('Dashboard');
  t('Edit Profile');
  t('Events');
  t('Forum');
  t('Log out');
  t('Login');
  t('Logout');
  t('Messages');
  t('My !points');
  t('My Account');
  t('News');
  t('Register');
  t('User account');
  t('View Profile');


  return $menu_links;
}
