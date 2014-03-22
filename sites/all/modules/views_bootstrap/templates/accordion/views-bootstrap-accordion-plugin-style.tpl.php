<div id="views-bootstrap-accordion-<?php print $id ?>" class="<?php print $classes ?>">
  <?php foreach ($rows as $key => $row): ?>
    <div class="accordion-group">
      <div class="accordion-heading">
        <a class="accordion-toggle"
           data-toggle="collapse"
           data-parent="#views-bootstrap-accordion-<?php print $id ?>"
           href="#collapse<?php print $key ?>">
          <?php print $titles[$key] ?>
        </a>
      </div>

      <div id="collapse<?php print $key ?>" class="accordion-body collapse">
        <div class="accordion-inner">
          <?php print $row ?>
        </div>
      </div>
    </div>
  <?php endforeach ?>
</div>