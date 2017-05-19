<?php

/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<div id="node-<?php print $node->nid; ?>" class="equus-list <?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="field-content"<?php print $content_attributes; ?>>
    <a href="<?php print $node_url; ?>"><?php print render($tile_image); ?></a>

    <div class="content-info">
		<div class="vocab">
			<?php if ($node->type == 'blog') { print render($blog_categories); } ?>
			<?php if ($node->type == 'organization') { print render($org_type); } ?>
			<?php if ($node->type == 'horse') { print render($status); } ?>
			<?php if ($node->type == 'property'): ?>
				<div class="property-type">
					<?php print render($property_type); ?>
				</div>
			<?php endif; ?>
			<?php if ($node->type == 'equus_sale'): ?>
				<?php if ($sale_type['#items'][0]['value'] != 'gameplayitem'): ?>
					<?php { print render($item_type); } ?>
				<?php endif; ?>
			<?php endif; ?>
		</div>
	    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>">
	    <?php if ($node->type == 'horse') {
	    	print render($real_name);
	    } else {
	    	print $title;
	    } ?></a></h2>
	    <div class="post-info">
	    	<?php if ($node->type == 'blog') {
	    		print 'Posted by ' . $name . ' on ' . $submitted;
	    	} ?>
	    	<?php if ($node->type == 'horse') {
	    		print 'Owned by ' . $name;
	    		print '<br><span class="horse-info">'
	    			. render($content['field_breed'])
	    			. ' '
	    			. render($content['field_horse_gender'])
	    			. '</span>';
	    	} ?>
	    </div>
	    <?php if ($node->type == 'blog' || $node->type == 'equus_sale' || $node->type == 'property') {
			print render($body_teaser);
		} ?>
		<?php if ($node->type == 'organization'): ?>
<!--			<span class="org-label">Prefix</span>-->
<!--			<span class="org-value">--><?php //print render($prefix); ?><!--</span>-->
			<?php print render($mission_summary); ?>
		<?php endif; ?>
		<div class="post-footer">
		<?php
		  if (!empty($node->field_blog_tags)) {
		    print '<span class="tags">';
		    print '<span class="icon"></span>';
		    foreach($node->field_blog_tags['und'] as $tag) {
		      $term = taxonomy_term_load($tag['tid']);
		      if ($term->vocabulary_machine_name == 'blog_tags') {
		        print l($term->name, "taxonomy/term/{$term->tid}").' ';
		      }
		    };
		    print '</span>';
		  }
		?>
          <?php if ($node->type == 'equus_sale'): ?>
              <?php print render($price_per_unit); ?>
          <?php endif; ?>
	  	<?php if ($node->type == 'organization'): ?>
	  		<span class="org-funds">
				<?php print l($bank_balance,$bank_transactions_path); ?>
			</span>
	  	<?php endif; ?>
		  <?php if ($node->type == 'horse'): ?>
			<?php print '<span class="tags">'; ?>
			<?php print render($disciplines); ?>
			<?php print '</span>'; ?>
		  <?php endif; ?>
		<div class="footer-link">
			<?php print flag_create_link("likes", $node->nid); ?>
			<a class="icon footer-readmore" href="<?php print $node_url; ?>"></a>
		</div>
	</div>
	</div>
  </div>
</div>