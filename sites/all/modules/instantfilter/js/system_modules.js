(function ($, Drupal) {

  "use strict";

  /**
   * Filters the module list table by a text input search string.
   *
   * Additionally accounts for multiple tables being wrapped in "package" details
   * elements.
   *
   * Text search input: input.table-filter-text
   * Target table:      input.table-filter-text[data-table]
   * Source text:       .table-filter-text-source
   */
  Drupal.behaviors.tableFilterByText = {
    attach: function (context, settings) {
      var $input = $('input.table-filter-text').once('table-filter-text');
      var $table = $($input.attr('data-table'));
      var $rowsAndDetails, $rows, $details;
      var searching = false;

      function hidePackageDetails(index, element) {
        var $packDetails = $(element);
        var $visibleRows = $packDetails.find('table:not(.sticky-header)').find('tbody tr:visible');
        $packDetails.toggle($visibleRows.length > 0);
      }

      function expandFieldset(index, element) {
        Drupal.toggleFieldset(element);
      }

      function filterModuleList(e) {
        var query = $(e.target).val().toLowerCase();

        function showModuleRow(index, row) {
          var $row = $(row);
          var $sources = $row.find('.table-filter-text-source');
          var textMatch = $sources.text().toLowerCase().indexOf(query) !== -1;
          $row.closest('tr').toggle(textMatch);
        }

        // Filter if the length of the query is at least 2 characters.
        if (query.length >= 2) {
          searching = true;
          $details.filter('.collapsed').each(expandFieldset);
          $rows.each(showModuleRow);

          // Hide the package <details> if they don't have any visible rows.
          // Note that we first show() all <details> to be able to use ':visible'.
          $details.show().each(hidePackageDetails);
        }
        else if (searching) {
          searching = false;
          $rowsAndDetails.show();
        }
      }

      if ($table.length) {
        $rowsAndDetails = $table.find('tr, fieldset');
        $rows = $table.find('tbody tr');
        $details = $table.find('fieldset');

        $input.bind('input', filterModuleList);
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
