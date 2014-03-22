<?php print $image ?>

<?php if (!empty($title) || !empty($description)): ?>
  <div class="carousel-caption">
    <?php if (!empty($title)): ?>
      <h4><?php print $title ?></h4>
    <?php endif ?>

    <?php if (!empty($description)): ?>
      <p><?php print $description ?></p>
    <?php endif ?>
  </div>
<?php endif ?>