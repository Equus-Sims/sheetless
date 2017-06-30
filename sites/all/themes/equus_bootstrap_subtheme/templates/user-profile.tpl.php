<?php

/**
 * @file
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * Use render($user_profile) to print all profile items, or print a subset
 * such as render($user_profile['user_picture']). Always call
 * render($user_profile) at the end in order to print all remaining items. If
 * the item is a category, it will contain all its profile items. By default,
 * $user_profile['summary'] is provided, which contains data on the user's
 * history. Other data can be included by modules. $user_profile['user_picture']
 * is available for showing the account picture.
 *
 * Available variables:
 *   - $user_profile: An array of profile items. Use render() to print them.
 *   - Field variables: for each field instance attached to the user a
 *     corresponding variable is defined; e.g., $account->field_example has a
 *     variable $field_example defined. When needing to access a field's raw
 *     values, developers/themers are strongly encouraged to use these
 *     variables. Otherwise they will have to explicitly specify the desired
 *     field language, e.g. $account->field_example['en'], thus overriding any
 *     language negotiation rule that was previously applied.
 *
 * @see user-profile-category.tpl.php
 *   Where the html is handled for the group.
 * @see user-profile-item.tpl.php
 *   Where the html is handled for each item in the group.
 * @see template_preprocess_user_profile()
 *
 * @ingroup themeable
 */
?>
<div class="profile"<?php print $attributes; ?>>
	<div class="user-sub-header">
		<div class="user-sub-header-block">
			<div class="title">
				<span class="roundImg">
					<?php echo render($user_profile['user_picture']); ?>
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
				<span class="profile-label">Achievements</span>
				<span class="profile-value">0</span>
			</div>
		</div>
	</div>
	<div class="user-profile-content">
		<div class="user-profile-content-about">
			<div class="content-label">About Me</div>
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
			<div class="content-label">Biography</div>
			<span class="profile-value">
				<?php if (isset($user_biography)): ?>
					<?php echo $user_biography; ?>
				<?php endif; ?>
			</span>
		</div>
	</div>
</div>
