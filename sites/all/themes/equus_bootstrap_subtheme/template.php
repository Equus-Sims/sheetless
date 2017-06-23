<?php

/**
 * @file
 * template.php
 */

function equus_bootstrap_subtheme_form_comment_form_alter(&$form, &$form_state, &$form_id) {
  $form['comment_body']['#after_build'][] = '_equus_bootstrap_subtheme_customize_comment_form';
}

function _equus_bootstrap_subtheme_customize_comment_form(&$form) {
  $form[LANGUAGE_NONE][0]['format']['#access'] = FALSE;
  return $form;
}

function equus_bootstrap_subtheme_preprocess_block(&$vars) {
	$vars['full_content_name'] = "";
	$vars['full_content_path'] = "";

	if ($vars['block_html_id'] == 'block-views-latest-blog-entry-block') {
		$url = explode('/',$_GET['q']);
		$uid = $url[1];

	    $vars['full_content_name'] = "My Blog";
	    $vars['full_content_path'] = "user/$uid/blog";
	}

	if ($vars['block_html_id'] == 'block-views-newest-horse-view-block') {
		$url = explode('/',$_GET['q']);
		$uid = $url[1];

	    $vars['full_content_name'] = "My Horses";
	    $vars['full_content_path'] = "user/$uid/horses";
	}

	if ($vars['block_html_id'] == 'block-views-latest-gallery-image-block') {
		$url = explode('/',$_GET['q']);
		$uid = $url[1];

	    $vars['full_content_name'] = "My Album";
	    $vars['full_content_path'] = "user/$uid/gallery";
	}
}

function equus_bootstrap_subtheme_node_view_alter(&$build) {
  // Remove the read more link
  unset($build['links']['node']['#links']['node-readmore']);
  unset($build['links']['blog']['#links']['blog_usernames_blog']);
  unset($build['links']['comment']['#links']['comment-add']);
  unset($build['links']['flag']['#links']['flag-likes']);
  unset($build['links']['flag']['#links']['flag-bookmarks']);

  // Add your own custom link
  /*$build['links']['node']['#links']['example-mylink'] = array(
    'title' => t('Test link'), 
    'href' => 'foo', 
    'html' => TRUE, 
    'attributes' => array(
      'title' => 'Test link',
    ),
  );*/

  // Move read more link to first slot
  /*$link_read_more = $build['links']['node']['#links']['node_read_more'];
  unset($build['links']['node']['#links']['node_read_more']);
  $links = $build['links']['node']['#links'];
  $build['links']['node']['#links'] = array(
    'node_read_more' => $link_read_more,
  ) + $links;

  // Move link to the last slot
  $link_read_more = $build['links']['node']['#links']['node_read_more'];
  unset($build['links']['node']['#links']['node_read_more']);
  $build['links']['node']['#links']['node_read_more'] = $link_read_more;*/
}

function equus_bootstrap_subtheme_preprocess_page(&$vars) {
	 $alias = drupal_get_path_alias();

	$menu = menu_navigation_links('main-menu');
    // foreach ($menu as &$item) {
    //     if (isset($item['attributes']) && isset($item['attributes']['class']) && in_array('home', $item['attributes']['class'])) {
    //         $item['html'] = true;
    //         $item['title'] = "<img src=" . base_path() . path_to_theme() . "/horseshoe-nearblack.svg />";
    //     }
    // }

	$vars['main_menu'] = theme('links__system_main_menu', array('links' => $menu));

    // ICONS 
	$vars['title_logo'] = "<img src=" . base_path() . path_to_theme() . "/horseshoe-white.svg />";
	$vars['settings_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/settings-white.svg />";
	$vars['dashboard_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/activity-white.svg />";
	$vars['messages_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/mail-white.svg />";
	$vars['bell_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/bell-white.svg />";
	$vars['credit_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/diamond.svg />";
	$vars['chevron_down_icon'] = "<img class ='sidebar-chevron expand' src=" . base_path() . path_to_theme() . "/icons/chevron-down-white.svg />";
	$vars['chevron_up_icon'] = "<img class ='sidebar-chevron' src=" . base_path() . path_to_theme() . "/icons/chevron-up-white.svg />";
	$vars['plus_circle_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/plus-circle-white.svg />";
	$vars['log_in_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/log-in-white.svg />";
	$vars['log_out_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/log-out-white.svg />";
	$vars['dollar_icon'] = "<img class ='sidebar-dollar' src=" . base_path() . path_to_theme() . "/icons/dollar.svg />";
	$vars['credit_card_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/credit-card-white.svg />";
	$vars['register_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/user-plus-white.svg />";

	// END ICONS

	if (arg(0, $alias) == 'user' && arg(1, $alias) == 'login') {
		$vars['theme_hook_suggestions'][] = 'page__login';
	}

	if (arg(0, $alias) == 'user' && arg(1, $alias) == 'register') {
		$vars['theme_hook_suggestions'][] = 'page__register';
	}

	 if (arg(0,$alias) == 'user' && arg(2,$alias) == 'profile') {
	 	$uid = arg(1,$alias);
	 	$user = user_load($uid);
		$username = $user->name;

		$recipients = $user;
		$vars['message'] = privatemsg_get_link($recipients);

	 	$vars['equus_type'] = "Member";
	 	$vars['creation_date'] = date("M j, Y", $user->created);
	 } else if (isset($vars['node']) AND ($vars['node']->type != 'page')) {
	 	$vars['draw_sub_header'] = true;
		$vars['title'] = $vars['node']->title;
		$vars['equus_type'] = $vars['node']->type;
		$vars['creation_date'] = date("M j, Y", $vars['node']->created);
		// check if node type is one of the ones with special title
		// set variables for special title fields
	} else {
		$vars['draw_sub_header'] = false;
	}

	//Render correct column widths
	if (!empty($vars['page']['sidebar_first']) && !empty($vars['page']['sidebar_second'])) {
		$vars['content_column_class'] = ' class="col-sm-6"';
	}
	elseif (!empty($vars['page']['sidebar_first']) || !empty($vars['page']['sidebar_second'])) {
		$vars['content_column_class'] = ' class="col-sm-8"';
	}
	elseif (!empty($vars['page']['sidebar_small'])) {
		$vars['content_column_class'] = ' class="col-sm-9"';
	}
	else {
		$vars['content_column_class'] = ' class="col-sm-12"';
	}

    $uid = $GLOBALS['user']->uid;
    if ($uid == 0) {
		$vars['logged_in'] = false;
	} else {
		$user = user_load($uid);
        $vars['uid'] = $uid;
        $vars['logged_in'] = true;
        $vars['name'] = format_username($GLOBALS['user']);
        if (isset($user->picture)) {
        	$vars['user_picture'] = "<img src='" . file_create_url($user->picture->uri) . "' />";
        }
        setlocale(LC_MONETARY, 'en_US');
		$vars['net_worth'] = money_format('%.0n', $user->field_equus_user_net_worth['und'][0]['value']);
		$vars['orgs'] = array();
		$nids = equus_organizations_get_assoc_orgs($user->uid);
		if (!empty($nids)) {
			$orgs = node_load_multiple($nids);
			foreach ($orgs as $org) {
				$org_info = array();
				$org_info['name'] = $org->title;
				$org_info['path'] = "node/{$org->nid}";
				$org_info['bank_balance'] = money_format('%.0n', $org->equus_organizations_balance['und'][0]['value']);
				$org_info['bank_transactions_path'] = "organization/transactions/{$org->nid}";

				$vars['orgs'][] = $org_info;
			}
		}
        $vars['regular_credit'] = userpoints_get_current_points($user->uid, 1);
        $vars['rare_credit'] = userpoints_get_current_points($user->uid, 2);
        $vars['org_credit'] = userpoints_get_current_points($user->uid, 60);
        $vars['total_credits'] = $vars['regular_credit'] + $vars['rare_credit'] + $vars['org_credit'];
	}

//	// what follows is the tale of the toolbar
//	$vars_toolbar = array();
//	$vars_toolbar['search_box'] = drupal_get_form('search_form');
//	if ($GLOBALS['user']->uid == 0) {
//		$vars_toolbar['logged_in'] = false;
//	} else {
//		$user = user_load($GLOBALS['user']->uid);
//		$vars_toolbar['logged_in'] = true;
//		$vars_toolbar['name'] = format_username($GLOBALS['user']);
//		$vars_toolbar['uid'] = $user->uid;
//		setlocale(LC_MONETARY, 'en_US');
//		$vars_toolbar['net_worth'] = money_format('%.0n', $user->field_equus_user_net_worth['und'][0]['value']);
//		$vars_toolbar['orgs'] = array();
//		$nids = equus_organizations_get_assoc_orgs($user->uid);
//		if (!empty($nids)) {
//			$orgs = node_load_multiple($nids);
//			foreach ($orgs as $org) {
//				$org_info = array();
//				$org_info['name'] = $org->title;
//				$org_info['path'] = "node/{$org->nid}";
//				$org_info['bank_balance'] = money_format('%.0n', $org->equus_organizations_balance['und'][0]['value']);
//				$org_info['bank_transactions_path'] = "organization/transactions/{$org->nid}";
//
//				$vars_toolbar['orgs'][] = $org_info;
//			}
//		}
//		$vars_toolbar['regular_credit'] = userpoints_get_current_points($user->uid, 1);
//		$vars_toolbar['rare_credit'] = userpoints_get_current_points($user->uid, 2);
//		$vars_toolbar['org_credit'] = userpoints_get_current_points($user->uid, 60);
//		$vars_toolbar['total_credits'] = $vars_toolbar['regular_credit'] + $vars_toolbar['rare_credit'] + $vars_toolbar['org_credit'];
//	}
//
//	$vars_toolbar['foo'] = print_r($GLOBALS['user'], true);
//	$vars['toolbar'] = theme('equus_toolbar', $vars_toolbar);
}

function equus_bootstrap_subtheme_preprocess_node(&$vars)
{
	$vars['base_path'] = base_path();

	$vars['theme_hook_suggestions'][] = 'node__' . $vars['view_mode'];

	if ($vars['node']->type == 'property' && isset($vars['field_user_property_type']['und'])) {
		$vars['property_type'] = $vars['field_user_property_type']['und'][0]['value'];
	}

	if ($vars['node']->type == "organization") {
		$org = node_load($vars['node']->nid);
		setlocale(LC_MONETARY, 'en_US');

		if ($vars['view_mode'] == 'tiles') {
			$vars['prefix'] = $vars['equus_organizations_prefix']['und'][0]['value'];

			$org_type = field_view_field('node', $vars['node'], 'equus_organizations_type', array('type' => 'taxonomy_term_reference_plain'));
			$org_type['#label_display'] = 'hidden';
			$vars['org_type'] = $org_type;

			if (isset($vars['equus_organizations_mission']['und'])) {
				$vars['mission_summary'] = text_summary($vars['equus_organizations_mission']['und'][0]['value'], NULL, 400);
				$vars['mission_summary'] .= '...' . l("read more", "node/{$vars['node']->nid}", array(
						'attributes' => array(
							'class' => array(
								'link-readmore'))));
			}
		}

		$vars['bank_balance'] = money_format('%.0n', $org->equus_organizations_balance['und'][0]['value']);
		$vars['bank_transactions_path'] = "organization/transactions/{$vars['node']->nid}";
	}

	$view_mode = $vars['view_mode'];
	if ($view_mode == 'tiles') {
		$image_style = 'tile';
	} else {
		$image_style = 'blog_cover_image';
	}

	if ($vars['node']->type == 'blog') {
		// set up render array for cover image
		$image = field_get_items('node', $vars['node'], 'field_cover_image');
		if (!empty($image)) {
			$image = field_view_value('node', $vars['node'], 'field_cover_image', $image[0], array(
				'type' => 'image',
				'settings' => array('image_style' => $image_style)
			));
		}
		$vars['cover_image'] = $image;
	} else if ($vars['node']->type == 'property') {
		// set up render array for cover image
		$image = field_get_items('node', $vars['node'], 'field_user_property_cover_image');
		if (!empty($image)) {
			$image = field_view_value('node', $vars['node'], 'field_user_property_cover_image', $image[0], array(
				'type' => 'image',
				'settings' => array('image_style' => $image_style)
			));
		}
		$vars['cover_image'] = $image;
	} else {
		// set up render array for cover image
		$image = field_get_items('node', $vars['node'], 'field_cover_image');
		if (!empty($image)) {
			$image = field_view_value('node', $vars['node'], 'field_cover_image', $image[0], array(
				'type' => 'image',
				'settings' => array('image_style' => $image_style)
			));
		}
		$vars['cover_image'] = $image;
	}

//    $image = field_get_items('node', $vars['node'], 'field_cover_image');
//    if (!empty($image)) {
//        $image = field_view_value('node', $vars['node'], 'field_cover_image', $image[0], array(
//          'type' => 'image',
//          'settings' => array('image_style' => 'tile')
//        ));
//    }
    $vars['tile_image'] = $image;

    if ($vars['node']->type == 'blog') {
	    // set up render array for blog categories
	    $blog_categories = field_view_field('node',$vars['node'],'field_blog_categories');
	    $blog_categories['#label_display'] = 'hidden';
	    $vars['blog_categories'] = $blog_categories;
	}

	$vars['submitted'] = date("M j, Y", $vars['created']);

	$body = field_get_items('node', $vars['node'], 'body');
	if ($vars['view_mode'] == 'tiles') {
		$vars['body_teaser'] = text_summary($body[0]['value'], NULL, 150);
		$vars['body_teaser'] .= '...' . l("read more", "node/{$vars['node']->nid}", array(
			'attributes' => array(
				'class' => array(
					'link-readmore'))));
	} else {
		$vars['body_teaser'] = text_summary($body[0]['value'], NULL, 400);
	}

	if ($vars['node']->type == 'equus_sale') {
		$hosting_org = node_load($vars['node']->field_hosting_organization['und'][0]['target_id']);
		$vars['hosting_org_path'] = 'node/$hosting_org';
		$vars['hosting_org'] = $hosting_org->title;
	    // set up render array for sale categories and price
        $sale_type = field_view_field('node', $vars['node'], 'field_equus_sale_type');
        $sale_type['#label_display]'] = 'hidden';
        $vars['sale_type'] = $sale_type;

        $item_type = field_view_field('node', $vars['node'], 'field_equus_sale_item_type');
        $item_type['#label_display]'] = 'hidden';
        $vars['item_type'] = $item_type;

	    $price_per_unit = field_view_field('node',$vars['node'],'field_equus_sale_price_per_unit',
	    	'default');
	    $price_per_unit['#label_display'] = 'hidden';
	    $vars['price_per_unit'] = $price_per_unit;

		$item_nid = field_get_items('node', $vars['node'], 'field_equus_sale_item');
		$vars['item_nid'] = $item_nid[0]['target_id'];
	}

	if ($vars['node']->type == 'horse') {
		//	$status = field_view_field('node', $vars['node'], 'field_horse_status');
		$status = 'Training';
		// $status['#label_display'] = 'hidden';
		$vars['status'] = $status;

	    $real_name = field_view_field('node',$vars['node'],'field_horse_real_name');
	    $real_name['#label_display'] = 'hidden';
	    $vars['real_name'] = $real_name;
	    $vars['title'] = $real_name;

//		$disciplines = field_view_field('node',$vars['node'],'field_horse_discipline');
//		$disciplines['#label_display'] = 'hidden';
//		$vars['disciplines'] = $disciplines;
		$vars['disciplines'] = "Barrels and Poles";
	}

	if ($vars['node']->type == 'show') {
		$show_level = $vars['node']->field_show_level['und'][0]['value'];
		$vars['level'] = $show_level;

		// set up render array for show type
		$show_type = field_view_field('node',$vars['node'],'field_show_type');
	    $show_type_value = $show_type['#items'][0]['value'];
	    $vars['show_type'] = ucfirst($show_type_value);
	    
	    switch ($show_type_value) {
    		case 'simulated':
        		$show_type_path = 'node/1200';
        		break;
    		case 'roleplay':
        		$show_type_path = 'node/1203';
        		break;
    		case 'randomized':
        		$show_type_path = 'node/1244';
        		break;
    		case 'creative':
    			$show_type_path = 'node/1243';
    			break;
		}
	    
	    $vars['show_type_path'] = $show_type_path;

		// set up render array for discipline term
	    $discipline = field_view_field('node',$vars['node'],'field_discipline');
	    $discipline['#label_display'] = 'hidden';
	    $vars['discipline'] = $discipline;

	    $show_status = $vars['node']->field_show_status['und'][0]['value'];
	    switch ($show_status) {
	    	case 'open':
    			$vars['show_status'] = 'Open';
    			break;
    		case 'in_progress':
    			$vars['show_status'] = 'In Progress';
    			break;
    		case 'submissions':
    			$vars['show_status'] = 'Submissions';
    			break;
			case 'completed':
    			$vars['show_status'] = 'Completed';
    			break;
    		case 'new_phase':
    			$vars['show_status'] = 'New Phase';
    			break;
			default:
				$vars['show_status'] = $show_status;
		}

		$open_date_raw = new DateTime($vars['node']->field_show_open_date['und'][0]['value']);
		$open_date = $open_date_raw->format('M. d, Y');
		$vars['open_date'] = $open_date;

		$current_date = new DateTime();

		if ($open_date_raw <= $current_date) {
			$vars['open_label'] = 'Opened';
		} else {
			$vars['open_label'] = 'Opens';
		}

	    // get the body text
	    $body = field_view_field('node',$vars['node'],'body');
	    $body['#label_display'] = 'hidden';
	    $vars['body'] = $body;

	    // get the host
	    $host = field_view_field('node',$vars['node'],'field_hosting_organization');
	    $host_org_name = $host[0]['#markup'];
	    $vars['host'] = $host_org_name;

	    // get the host org path
	    $host_org_id = $host['#items'][0]['target_id'];
	    $host_org_path = 'node/$host_org_id';
	    $vars['host_org_path'] = $host_org_path;

	    $entry_form = drupal_get_form('equus_shows_entry_form');
  		$vars['entry_form'] = $entry_form;

  		// TODO: Remove this when automatic show running is done
  		$compute_form = drupal_get_form('equus_shows_compute_form');
  		$vars['compute_form'] = $compute_form;

  		// $show_results = views_embed_view('show_results', 'block_1');
  		// dpm($show_results);
  		// $vars['show_results'] = $show_results;
	}
}

function equus_bootstrap_subtheme_get_content_bottom($view_mode) {
	if ($view_mode == "full") {
    	$content_bottom_region = block_get_blocks_by_region('content_bottom');
	    if (!$content_bottom_region) {
	    	$content_bottom_region = array();
	    }
    } else {
    	$content_bottom_region = array();
    }
    return $content_bottom_region;
}	

function equus_bootstrap_subtheme_preprocess_flag(&$vars) {
    // uncomment this line and equus_boostrap_subtheme_get_like_count and comment the line below for like counts on equus tiles
	// $vars['link_text'] = "<span class='icon {$vars['status']}'></span><span class='count'>".equus_bootstrap_subtheme_get_like_count($vars['entity_id'])."</span>";

    $vars['link_text'] = "<span class='icon {$vars['status']}'></span>";
}

//function equus_bootstrap_subtheme_get_like_count($nid) {
//	$result = flag_get_counts('node', $nid);
//
//	if (isset($result['likes'])) {
//		return $result['likes'];
//	} else {
//		return 0;
//	}
//}

function equus_bootstrap_subtheme_preprocess_user_profile(&$vars) {
	$alias = drupal_get_path_alias();

	$profile_uid = arg(1,$alias);

	$u = user_load($profile_uid);

	$vars['profile_uid'] = $profile_uid;

	$vars['realname'] = $u->realname;

	$vars['user_role'] = print_r($u, true);

	$created_date = new DateTime('@' . $u->created);
	$vars['created'] = $created_date->format('M. d, Y');

	if (arg(0,$alias) == 'user' && arg(2,$alias) == 'profile') {
		$vars['user_profile_counters'] = true;
	} else {
		$vars['user_profile_counters'] = false;
	}

	$vars['content_bottom_region'] = equus_bootstrap_subtheme_get_content_bottom("full");
}

function equus_bootstrap_subtheme_preprocess_field(&$vars) {
	if ($vars['element']['#field_name'] == 'field_horse_stats') {
		$stats = json_decode($vars['element']['#items'][0]['value']);
		foreach($stats as $stat => $value) {
			$value->potential = round($value->potential, 0);
		}
		$vars['stats'] = $stats;
	}
}

function equus_bootstrap_subtheme_preprocess_views_view(&$vars) {
    if ($vars['view']->name == 'user_blog' || $vars['view']->name == 'organizations' || $vars['view']->name == 'puppies') {
		$alias = drupal_get_path_alias();

		$profile_uid = arg(1,$alias);

		$u = user_load($profile_uid);
		// dpm($u);

		if ($vars['view']->name == 'user_blog') {
			$vars['blog_active'] = "class='active-trail active'";
		} else {
			$vars['blog_active'] = "";
		}
		if ($vars['view']->name == 'organizations') {
			$vars['orgs_active'] = "class='active-trail active'";
		} else {
			$vars['orgs_active'] = "";
		}
		if ($vars['view']->name == 'puppies') {
			$vars['horses_active'] = "class='active-trail active'";
		} else {
			$vars['horses_active'] = "";
		}
		$dob_raw = new DateTime($u->field_user_dob['und'][0]['value']);
		$dob = $dob_raw->format('M. d, Y');
		$interval = $dob_raw->diff(new DateTime());
		$vars['user_age'] = $interval->y;
		$vars['user_dob'] = $dob;

		$vars['cover_photo'] = array(
			'style_name' => 'hero_image_style',
			'path' => $u->field_cover_image['und'][0]['uri'],
			'alt' => 'User Profile Cover Image',
			'title' => 'User Profile Cover Image'
		);

		$vars['user_picture'] = "<img src='" . file_create_url($u->picture->uri) . "' />";

		$vars['profile_uid'] = $profile_uid;

		$vars['realname'] = $u->realname;

		$vars['user_role'] = print_r($u, true);

		$vars['user_location'] = $u->field_user_location['und'][0]['value'];

		$created_date = new DateTime('@' . $u->created);
		$vars['created'] = $created_date->format('M. d, Y');

		$vars['user_biography'] = $u->field_user_biography['und'][0]['safe_value'];

		setlocale(LC_MONETARY, 'en_US');
		$vars['user_net_worth'] = money_format('%.0n', $u->field_equus_user_net_worth['und'][0]['value']);
	}
}

function equus_bootstrap_subtheme_preprocess_region(&$vars) {
	if ($vars['region'] == 'footer') {
		$vars['facebook_icon'] = "<img class ='footer-icon' src=" . base_path() . path_to_theme() . "/icons/facebook-grey.svg />";
		$vars['tumblr_icon'] = "<img class ='footer-icon' src=" . base_path() . path_to_theme() . "/icons/tumblr-grey-thick.svg />";
			// get season for icon in sidebar
		$season = equus_core_get_es_season();

		switch ($season) {
			// winter
			case '0':
				$vars['season_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/snowflake-o.svg />";
				break;
			// spring
			case '1':
				$vars['season_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/flower-white.svg />";
				break;
			// summer
			case '2':
				$vars['season_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/sun-white.svg />";
				break;
			// fall
			case '3':
				$vars['season_icon'] = "<img class ='sidebar-icon' src=" . base_path() . path_to_theme() . "/icons/plant-white.svg />";
				break;
		}
	}
}

/**
* hook_form_search_block_form_alter
*/
function equus_bootstrap_subtheme_form_search_block_form_alter(&$form, &$form_state, $form_id) {
    $form['search_block_form']['#title_display'] = 'invisible'; // Toggle label visibilty
    $form['search_block_form']['#size'] = 40;  // define size of the textfield
    $form['search_block_form']['#default_value'] = t('Search'); // Set a default value for the textfield
    //$form['actions']['submit']['#value'] = t('GO!'); // Change the text on the submit button
    //$form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/icons/search-white.svg');

    // Add extra attributes to the text box
    $form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search';}";
    $form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search') {this.value = '';}";
    // Prevent user from searching the default text
    $form['#attributes']['onsubmit'] = "if(this.search_block_form.value=='Search'){ alert('Please enter a search'); return false; }";
} 
