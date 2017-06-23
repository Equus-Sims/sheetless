<?php
/**
 * @file
 * Returns the HTML for the footer region.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728140
 */
?>
<?php if ($content): ?>
  <footer id="footer" class="<?php print $classes; ?>">
  	<span id="equus-season-icon"><?php print $season_icon; ?></span>
  	<span id="equus-year-text"><?php print equus_core_get_es_pretty_year(); ?></span>
  	<span id="footer-content">
		<div id="navigate">
	    	<div class="footer-title">Navigate</div>
		    <ul>
		    	<li><?php print l("About Us", 'about'); ?></li>
		    	<li><?php print l("News", 'news'); ?></li>
		    	<li><?php print l("Store", 'store'); ?></li>
		    	<li><?php print l("Blogs", 'blogs'); ?></li>
		    	<li><?php print l("Events", 'events'); ?></li>
		    </ul>
	    </div>
	    <div id="directories">
	    	<div class="footer-title">Directories</div>
		    <ul>
		    	<li><?php print l("Members", 'member-directory'); ?></li>
		    	<li><?php print l("Staff", 'staff-directory'); ?></li>
		    	<li><?php print l("Organizations", 'organizations'); ?></li>
		    	<li><?php print l("Horses", 'horses'); ?></li>
		    	<li><?php print l("Sims", 'sims'); ?></li>
		    </ul>
	    </div>
	    <div id="connect">
	    	<div class="footer-title">Connect</div>
		    <ul>
		    	<li><?php print l("Contact Us", 'contact'); ?></li>
		    	<li><?php print l("Privacy", 'privacy'); ?></li>
		    	<li><?php print l("Terms & Conditions", 'terms'); ?></li>
		    </ul>
		    <span id="social-media">
		    	<?php print l("$facebook_icon", 'https://www.facebook.com/equus.community', array('html' => TRUE)); ?>
		    	<?php print l("$tumblr_icon", 'http://equus-community.tumblr.com', array('html' => TRUE)); ?>
	    	</span>
	    </div>
	    <div id="recent-news">
	  		<div class="footer-title">Recent News</div>
		    <?php print views_embed_view('footer_news_block', 'block'); ?>
		</div>
	</span>
  </footer>
<?php endif; ?>
