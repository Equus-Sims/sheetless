(function ($, Drupal) {

  "use strict";

  /**
   * Filters the permission list table by a text input search string.
   *
   * Text search input: input.table-filter-text
   * Target table:      input.table-filter-text[data-table]
   * Source text:       .table-filter-text-source
   */
  Drupal.behaviors.tableFilterByText = {
    attach: function (context, settings) {
      var $input = $('input.table-filter-text').once('table-filter-text');
      var $table = $($input.attr('data-table'));
      var $rows;
      var searching = false;

      function filterPermissionList(e) {
        var query = $(e.target).val().toLowerCase();

        function showPermissionRow(index, row) {
          var $row = $(row);
          var $sources = $row.find('.table-filter-text-source');
          var textMatch = $sources.text().toLowerCase().indexOf(query) !== -1;
          $row.closest('tr').toggle(textMatch);
          if (textMatch) {
            $row.closest('tr').prevAll('tr:has(td.module)').first().toggle(textMatch);
          }
        }

        // Filter if the length of the query is at least 2 characters.
        if (query.length >= 2) {
          searching = true;
          $rows.each(showPermissionRow);
        }
        else if (searching) {
          searching = false;
          $rows.show();
        }
      }

      if ($table.length) {
        $rows = $table.find('tbody tr');

        $input.bind('input', filterPermissionList);
      }

      // Prevent form submission when hitting enter
      $input.bind('keypress', function(e) {
        if ((e.keyCode || e.which) == 13) {
          return false;
        }
      });
    }
  };

}(jQuery, Drupal));
