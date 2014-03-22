<?php
/**
 * @file views-bootstrap-grid-plugin-style.tpl.php
 * Default simple view template to display Bootstrap Grids.
 *
 *
 * - $columns contains rows grouped by columns.
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 * - $column_type contains a number (default Bootstrap grid system column type).
 *
 * @ingroup views_templates
 */
?>

<div id="views-bootstrap-grid-<?php print $id ?>" class="<?php print $classes ?>">
  <?php foreach ($columns as $column): ?>
    <div class="row-fluid">
      <?php foreach ($column as $key => $row): ?>
        <div class="span<?php print $column_type ?>">
          <?php print $row ?>
        </div>
      <?php endforeach ?>
    </div>
  <?php endforeach ?>
</div>
