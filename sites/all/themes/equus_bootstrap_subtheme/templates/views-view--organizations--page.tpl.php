<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */
?>
<div class="user-sub-header">
  <div class="user-sub-header-block">
    <div class="title">
        <span class="roundImg">
          <?php echo render($user_picture); ?>
        </span>
      <div class="user-info">
        <h1><?php echo $realname; ?><a href="<?php echo base_path(); ?>messages/new/<?php echo $profile_uid; ?>?destination=user/<?php echo $profile_uid; ?>/profile" class="icon message"></a></h1>
        <span class="user-role">Member</span>
        <!--<span class="user-role"><?php echo $user_role; ?></span>-->
      </div>
    </div>
    <div class="stats">
      <span class="profile-label">Net Worth</span>
      <span class="profile-value"><?php echo $user_net_worth; ?></span>
      <span class="profile-label">Member Points</span>
      <span class="profile-value">0</span>
      <span class="profile-label">Achievements</span>
      <span class="profile-value">0</span>
    </div>
  </div>
</div>
<div class="user-profile-content">
  <div class="user-profile-content-about">
    <h2 class="content-header">About Me</h2>
    <div>
      <span class="profile-label">Birthday</span>
      <span class="profile-value"><?php echo $user_dob; ?></span>
    </div>
    <div>
      <span class="profile-label">Age</span>
      <span class="profile-value"><?php echo $user_age; ?> years old</span>
    </div>
    <div>
      <span class="profile-label">Location</span>
      <span class="profile-value"><?php echo $user_location; ?></span>
    </div>
    <div>
      <span class="profile-label">Joined</span>
      <span class="profile-value"><?php echo $created; ?></span>
    </div>
  </div>
  <div class="user-profile-content-bio">
    <h2 class="content-header">Biography</h2>
			<span class="profile-value">
				<?php if (isset($user_biography)): ?>
                  <?php echo $user_biography; ?>
                <?php endif; ?>
			</span>
  </div>
</div>
<div><a name="o" id="anchor"></a></div>
<div class="user-menu">
  <ul>
    <li><a <?php echo $blog_active; ?> href=<?php echo base_path() . "user/2/blog#b"; ?> >Blog</a></li>
    <li><a <?php echo $orgs_active; ?> href=<?php echo base_path() . "user/2/organizations#o"; ?> >Organizations</a></li>
    <li><a <?php echo $horses_active; ?> href=<?php echo base_path() . "user/2/horses#h"; ?> >Horses</a></li>
  </ul>
</div>
<div class="user-profile-content">
  <div class="<?php print $classes; ?>">
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <?php print $title; ?>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ($header): ?>
      <div class="view-header">
        <?php print $header; ?>
      </div>
    <?php endif; ?>

    <?php if ($exposed): ?>
      <div class="view-filters">
        <?php print $exposed; ?>
      </div>
    <?php endif; ?>

    <?php if ($attachment_before): ?>
      <div class="attachment attachment-before">
        <?php print $attachment_before; ?>
      </div>
    <?php endif; ?>

    <?php if ($rows): ?>
      <div class="view-content">
        <?php print $rows; ?>
      </div>
    <?php elseif ($empty): ?>
      <div class="view-empty">
        <?php print $empty; ?>
      </div>
    <?php endif; ?>

    <?php if ($pager): ?>
      <?php print $pager; ?>
    <?php endif; ?>

    <?php if ($attachment_after): ?>
      <div class="attachment attachment-after">
        <?php print $attachment_after; ?>
      </div>
    <?php endif; ?>

    <?php if ($more): ?>
      <?php print $more; ?>
    <?php endif; ?>

    <?php if ($footer): ?>
      <div class="view-footer">
        <?php print $footer; ?>
      </div>
    <?php endif; ?>

    <?php if ($feed_icon): ?>
      <div class="feed-icon">
        <?php print $feed_icon; ?>
      </div>
    <?php endif; ?>

  </div><?php /* class view */ ?>
</div>
