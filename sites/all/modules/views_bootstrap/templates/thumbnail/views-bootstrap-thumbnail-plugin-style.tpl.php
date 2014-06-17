<?php
/**
 * @file views-bootstrap-thumbnail-plugin-style.tpl.php
 * Default simple view template to display Bootstrap Thumbnails.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 * - $column_type contains a number (default Bootstrap grid system column type).
 *
 * @ingroup views_templates
 */
?>

<div id="views-bootstrap-thumbnail-<?php print $id ?>" class="<?php print $classes ?>">
  <?php if ($options['alignment'] == 'horizontal'): ?>

    <?php foreach ($items as $row): ?>
      <ul class="thumbnails">
        <?php foreach ($row['content'] as $column): ?>
          <li class="span<?php print $column_type ?>">
            <div class="thumbnail">
              <?php print $column['content'] ?>
            </div>
          </li>
        <?php endforeach ?>
      </ul>
    <?php endforeach ?>

  <?php else: ?>

    <ul class="thumbnails">
      <?php foreach ($items as $column): ?>
        <li class="span<?php print $column_type ?>">
          <?php foreach ($column['content'] as $row): ?>
            <div class="thumbnail">
              <?php print $row['content'] ?>
            </div>
          <?php endforeach ?>
        </li>
      <?php endforeach ?>
    </ul>

  <?php endif ?>
</div>
