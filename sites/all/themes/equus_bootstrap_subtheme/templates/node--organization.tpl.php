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

  <div class="organization"<?php print $attributes; ?>>
    <!-- <div id="hero"><?php print render($content['field_cover_image']); ?></div> -->
    <div class="org-sub-header">
      <div class="org-sub-header-block">
        <div class="title">
<!--				<span class="roundImg">-->
<!--					--><?php //echo render($user_profile['user_picture']); ?>
<!--				</span>-->
          <div class="user-info">
            <h1><?php echo $title; ?>
              <span class="edit-delete">
                <?php print l("$edit_icon", "node/{$node->nid}/edit", array('html' => TRUE)); ?>
                <?php print l("$delete_icon", "node/{$node->nid}/delete", array('html' => TRUE)); ?>
              </span>
            </h1>
            <span class="org-type"><?php echo "Organization | "; print render($content['equus_organizations_type']); ?></span>
          </div>
        </div>
        <div class="stats">
          <span class="org-label">Available Funds</span>
          <span class="org-funds"><?php print l($bank_balance,$bank_transactions_path); ?></span>
        </div>
      </div>
    </div>
    <div class="org-content">
      <div class="org-content-about">
        <h2 class="content-header">About</h2>
        <div>
          <span class="org-label">Owner</span>
          <span class="org-value"><?php echo $name; ?></span>
        </div>
        <div>
          <?php if (isset($content['equus_organizations_users'])): ?>
            <span class="org-label">Members</span>
            <span class="org-value"><?php print render($content['equus_organizations_users']); ?></span>
          <?php endif; ?>
        </div>
        <div>
          <?php if (isset($content['field_user_location'])): ?>
            <span class="org-label">Location</span>
            <span class="org-value"><?php print render($content['field_user_location']); ?></span>
          <?php endif; ?>
        </div>
        <div>
          <span class="org-label">Prefix</span>
          <span class="org-value"><?php print render($content['equus_organizations_prefix']); ?></span>
        </div>
      </div>
      <div class="org-content-mission">
        <h2 class="content-header">Mission Statement</h2>
			<span class="org-value">
				<?php if (isset($content['equus_organizations_mission'])): ?>
                  <?php print render($content['equus_organizations_mission']); ?>
                <?php endif; ?>
			</span>
      </div>
    </div>
    <div class="user-menu">
        <ul>
            <li><a class="active-trail active" href=<?php echo $node_url; ?> >Properties</a></li>
        </ul>
    </div>
      <div class="property-content">
          <?php
            echo views_embed_view('user_property_list', 'block_1');
          ?>
      </div>
  </div>
