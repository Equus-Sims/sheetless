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
}

function equus_bootstrap_subtheme_preprocess_node(&$vars) {
	$vars['theme_hook_suggestions'][] = 'node__' . $vars['view_mode'];

	if ($vars['node']->type == "organization") {
		$ledger = equus_banking_retrieve_ledger();

		setlocale(LC_MONETARY, 'en_US');
		$vars['bank_balance'] = money_format('%.0n',equus_banking_balance($ledger, $vars['node']->nid));
		$vars['bank_transactions_path'] = "organization/transactions/{$vars['node']->nid}";
	}

	// set up render array for cover image
	$image = field_get_items('node', $vars['node'], 'field_cover_image');
    if (!empty($image)) {
        $image = field_view_value('node', $vars['node'], 'field_cover_image', $image[0], array(
          'type' => 'image',
          'settings' => array('image_style' => 'cover_image')
        ));
    }
    $vars['cover_image'] = $image;

    $vars['content_bottom_region'] = equus_bootstrap_subtheme_get_content_bottom($vars['view_mode']);

    if ($vars['node']->type == 'blog') {
	    // set up render array for blog categories
	    $blog_categories = field_view_field('node',$vars['node'],'field_blog_categories');
	    $blog_categories['#label_display'] = 'hidden';
	    $vars['blog_categories'] = $blog_categories;
	}

	$vars['submitted'] = date("M j, Y", $vars['created']);

	$body = field_get_items('node', $vars['node'], 'body');
	$vars['body_teaser'] = text_summary($body[0]['value'], NULL, 400);

	if ($vars['node']->type == 'equus_sale') {
	    // set up render array for blog categories
	    $price_per_unit = field_view_field('node',$vars['node'],'field_equus_sale_price_per_unit');
	    $price_per_unit['#label_display'] = 'hidden';
	    $vars['price_per_unit'] = $price_per_unit;
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
	$vars['link_text'] = "<span class='icon {$vars['status']}'></span><span class='count'>".equus_bootstrap_subtheme_get_like_count($vars['entity_id'])."</span>";
}

function equus_bootstrap_subtheme_get_like_count($nid) {
	$result = flag_get_counts('node', $nid);

	if (isset($result['likes'])) {
		return $result['likes'];
	} else {
		return 0;
	}
}

function equus_bootstrap_subtheme_preprocess_user_profile(&$vars) {
	$alias = drupal_get_path_alias();

	$profile_uid = arg(1,$alias);

	$u = user_load($profile_uid);

	$vars['profile_uid'] = $profile_uid;

	$vars['realname'] = $u->realname;

	$vars['user_role'] = print_r($u, true);

	if (arg(0,$alias) == 'user' && arg(2,$alias) == 'profile') {
		$vars['user_profile_counters'] = true;
	} else {
		$vars['user_profile_counters'] = false;
	}

	$vars['content_bottom_region'] = equus_bootstrap_subtheme_get_content_bottom("full");
}