// $Id: openlogin.js,v 1.2 2010/08/06 12:34:27 seaji Exp $
$(document).ready(function(){

$('.provider').click(function(){
  key = $(this).find('img').attr('rel');
  if(Drupal.login.providers[key]['openid1']) {
   username = prompt('Enter username');
   if(username) {
   openid = Drupal.login.providers[key]['url_prefix'] + username + Drupal.login.providers[key]['url_suffix']
   $('#edit-openid-identifier').attr('value', openid);
   $('#edit-openid-identifier').focus();
   $('#user-login').submit();
   }
  }
  else if (Drupal.login.providers[key]['openid1'] === false) {
   $('#edit-openid-identifier').attr('value', Drupal.login.providers[key]['url_prefix']);
   $('#user-login').submit();
  }
});

$('#user-login #edit-name').click(function(){
  $('#edit-openid-identifier').attr('value', '');
});

$('#user-login #edit-submit').click(function(){
  $('#edit-openid-identifier').attr('value', '');
  $('#user-login').submit();
});

});
