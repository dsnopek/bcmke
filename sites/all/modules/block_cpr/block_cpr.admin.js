// $Id: block_cpr.admin.js,v 1.1.2.1 2010/01/27 17:02:42 njt1982 Exp $

Drupal.behaviors.blockDrag = function(context) {
  var table = $('table#block_cpr');
  var tableDrag = Drupal.tableDrag.block_cpr;

  // Add a handler for when a row is swapped, update empty statuses.
  tableDrag.row.prototype.onSwap = function(swappedRow) {
    checkEmptyStatus(table, this);
  };

  // A custom message for the roles block page specifically.
  Drupal.theme.tableDragChangedWarning = function () {
    return '<div class="warning">' + Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t("The changes to these roles will not be saved until the <em>Save</em> button is clicked.") + '</div>';
  };

  // Add a handler so when a row is dropped, update fields dropped into new status.
  tableDrag.onDrop = function() {
    dragObject = this;

    if ($(dragObject.rowObject.element).prev('tr').is('.status-message')) {
      // If the previous row is a status message, use this to get the status
      var statusRow = $(dragObject.rowObject.element).prev('tr').get(0);
      var statusName = statusRow.className.replace(/([^ ]+[ ]+)*status-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
    }
    else {
      // otherwise use the row above (specifically, the select element)
      var statusRow = $(dragObject.rowObject.element).prev('tr').get(0);
      var statusSelect = $('select.role-weight-element', statusRow).get(0);
      var statusName = statusSelect.className.replace(/([^ ]+[ ]+)*role-weight-element-([^ ]+)([ ]+[^ ]+)*/, '$2');
    }
    var statusField = $('input.role-status-element', dragObject.rowObject.element);
    var weightField = $('select.role-weight-element', dragObject.rowObject.element);

    var oldStatusName = weightField[0].className.replace(/([^ ]+[ ]+)*role-weight-element-([^ ]+)([ ]+[^ ]+)*/, '$2');
    if (!statusField.is('.role-status-'+ statusName)) {
      statusField.removeClass('role-status-element-' + oldStatusName).addClass('role-status-element-' + statusName);
      weightField.removeClass('role-weight-element-' + oldStatusName).addClass('role-weight-element-' + statusName);
      statusField.attr('checked', (statusName === 'enabled'));
    }
  };

  // Add the behavior to each status checkbox.
  $('input.role-status-element:not(.rolestatuselement-processed)', context).each(function() {
    $(this).change(function(event) {
      // Make our new row and select field.
      var row = $(this).parents('tr:first');
      var select = $(this);
      var status = select[0].checked == 1 ? 'enabled' : 'disabled';
      tableDrag.rowObject = new tableDrag.row(row);

      // Find the correct region and insert the row as the first in the region.
      $('tr.status-message', table).each(function() {
        if ($(this).is('.status-' + status + '-message')) {
          // Add the new row and remove the old one.
          $(this).after(row);
          // Manually update weights and restripe.
          tableDrag.updateFields(row.get(0));
          tableDrag.rowObject.changed = true;
          if (tableDrag.oldRowElement) {
            $(tableDrag.oldRowElement).removeClass('drag-previous');
          }
          tableDrag.oldRowElement = row.get(0);
          tableDrag.restripeTable();
          tableDrag.rowObject.markChanged();
          tableDrag.oldRowElement = row;
          $(row).addClass('drag-previous');
        }
      });

      // Modify empty regions with added or removed fields.
      checkEmptyStatus(table, row);
      // Remove focus from selectbox.
      select.get(0).blur();
    });
    $(this).addClass('rolestatuselement-processed');
  });

  // Empty status checked
  var checkEmptyStatus = function(table, rowObject) {
    $('tr.status-message', table).each(function() {
      // If the dragged row is in this status, but above the message row, swap it down one space.
      if ($(this).prev('tr').get(0) == rowObject.element) {
        // Prevent a recursion problem when using the keyboard to move rows up.
        if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
          rowObject.swap('after', this);
        }
      }
      // This status has become empty
      if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) {
        $(this).removeClass('status-populated').addClass('status-empty');
      }
      // This status has become populated.
      else if ($(this).is('.status-empty')) {
        $(this).removeClass('status-empty').addClass('status-populated');
      }
    });
  };
}
