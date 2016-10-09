/**
 * Created by potetofurai on 6/5/16.
 */

var equusHorsesExtrasCalcAge = function(dob, foal_speed, adult_speed, transition_year) {
    var dob_calcd = moment(dob);
    var today = moment();
    var real_days = today.diff(dob_calcd, "days");
    var transition_year = parseFloat(transition_year);
    var transition_days = transition_year * 365;

    var game_days_tentative = real_days * foal_speed;
    if (game_days_tentative < transition_days) {
        return game_days_tentative / 365;
    } else {
        var real_days_foal = transition_days / foal_speed;
        var real_days_adult = real_days - real_days_foal;
        return transition_year + (real_days_adult * adult_speed) / 365;
    }
};

var equusHorsesExtrasCalcDob = function(age, foal_speed, adult_speed, transition_year) {
    var today = moment();
    var age = parseFloat(age);
    var transition_year = parseFloat(transition_year);
    var transition_days = transition_year * 365;
    var real_days = 0;

    if (age < transition_year) {
        real_days = (age * 365) / foal_speed;
    } else {
        real_days = (transition_days / foal_speed) + ((age - transition_year) * 365) / adult_speed;
    }
    return today.subtract(real_days, "days").format("YYYY-MM-DD");
};

var equusHorsesExtrasToggleAgingSystem = function() {
    var $ = jQuery;

    if ($('#edit-temp-age').length > 1) {
        console.log("foo");
    }

    // $('#edit-temp-age').prop("disabled", false);
    console.log($('#edit-temp-age'));
};

(function($, Drupal, window, document, undefined) {
    var equusHorsesExtrasCalcAgeHelper = function() {
        var age = equusHorsesExtrasCalcAge(
            $('#edit-field-horse-date-of-birth-und-0-value-datepicker-popup-0').val(),
            $('#edit-foal-speed').val(),
            $('#edit-adult-speed').val(),
            $('#edit-transition-year').val()
        );
        $('#edit-temp-age').val(age);
    };

    var equusHorsesExtrasCalcDobHelper = function() {
        var dob = equusHorsesExtrasCalcDob(
            $('#edit-temp-age').val(),
            $('#edit-foal-speed').val(),
            $('#edit-adult-speed').val(),
            $('#edit-transition-year').val()
        );
        $('#edit-field-horse-date-of-birth-und-0-value-datepicker-popup-0').val(dob);
    };

    Drupal.behaviors.equus_horses_extras = {
        attach: function(context, settings) {
            equusHorsesExtrasCalcAgeHelper();
            $('#edit-field-horse-date-of-birth-und-0-value-datepicker-popup-0,#edit-foal-speed,#edit-adult-speed,#edit-transition-year').change(equusHorsesExtrasCalcAgeHelper);
            $('#edit-temp-age').change(equusHorsesExtrasCalcDobHelper);
        }
    };
}(jQuery, Drupal, this, document, undefined));
