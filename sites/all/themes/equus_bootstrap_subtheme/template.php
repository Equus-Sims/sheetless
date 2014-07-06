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
	$vars['foo'] = "foo not set";

	if (isset($vars['node'])) {
		// check if node type is one of the ones with special title
		if ($vars['node']->type == "organization") {
			$foo = "foo";
			$vars['foo'] = $foo;
		} 
		// set variables for special title fields
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

    if ($vars['node']->type == 'blog') {
	    // set up render array for blog categories
	    $blog_categories = field_view_field('node',$vars['node'],'field_blog_categories');
	    $blog_categories['#label_display'] = 'hidden';
	    $vars['blog_categories'] = $blog_categories;
	}

	$vars['submitted'] = date("M j, Y", $vars['created']);

	$body = field_get_items('node', $vars['node'], 'body');
	$vars['body_teaser'] = text_summary($body[0]['value'], NULL, 400);
}

function equus_bootstrap_subtheme_get_assoc_org($uid) {
	$query = new EntityFieldQuery();
	$query
		->entityCondition('entity_type', 'node')
		->entityCondition('bundle', 'organization')
		->propertyCondition('status', 1)
		->propertyCondition('uid', $uid);
	$result = $query->execute();
	$nid_owner = array();
	if (count($result) > 0) {
		$nid_owner = array_keys($result['node']);
	} 

	$query = new EntityFieldQuery();
	$query
		->entityCondition('entity_type', 'node')
		->entityCondition('bundle', 'organization')
		->propertyCondition('status', 1)
		->fieldCondition('field_co_owners', 'target_id', $uid);
	$result = $query->execute();
	$nid_co_owner = array();
	if (count($result) > 0) {
		$nid_co_owner = array_keys($result['node']);
	} 

	$nids = array_merge($nid_owner, $nid_co_owner);
	return $nids;
}

function equus_bootstrap_subtheme_get_net_worth($uid) {
	$ledger = equus_banking_retrieve_ledger();
	$nids = equus_bootstrap_subtheme_get_assoc_org($uid);
	
	$total = array_reduce($nids, function($carry, $item) use (&$ledger) {
		return $carry + equus_banking_balance($ledger, $item);
	}, 0);

	setlocale(LC_MONETARY, 'en_US');
	return money_format('%.0n', $total);
}

function equus_bootstrap_subtheme_preprocess_user_profile(&$vars) {
	$items = field_get_items('user', $vars['elements']['#account'], 'field_user_dob');
	$date = array_pop($items);
	
	$dob = new DateTime($date['value']);
	$interval = $dob->diff(new DateTime());
	$vars['age'] = $interval->y;
	
	$vars['net_worth'] = equus_bootstrap_subtheme_get_net_worth($vars['elements']['#account']->uid);
}