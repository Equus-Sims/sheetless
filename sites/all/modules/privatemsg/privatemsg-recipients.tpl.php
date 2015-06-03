<?php 
  //each file loads it's own styles because we cant predict which file will be loaded 
  drupal_add_css(drupal_get_path('module', 'privatemsg').'/styles/privatemsg-recipients.css');
?>
<div class="privatemsg-message-participants">
  <span><a href="#privatemsg-new">Post New Reply</a></span>
  <span class="participants"><?php print $participants; ?></span>
</div>