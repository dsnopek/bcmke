Testing Paypal
--------------
The paypal sandbox works just like regular paypal (with transactions and everything)
except no actual money gets transferred. It allows you to create mock accounts (seller
and buyer) and create transactions between them.

1. Sign up for an account at developer.paypal.com, receive the activation e-mail and
   activate the account
2. Log in, go to "test accounts" and create a business (seller) account and personal
   (buyer) account.
3. In whichever module you're using that uses simple_paypal, set the paypal e-mail
   address to your business account e-mail address.
4. In /admin/settings/paypal, be sure to select the paypal sandbox URL.

That's it. When a transaction has been completed, paypal should send a POST request to
the URL specified at the 'notify_url' variable.
