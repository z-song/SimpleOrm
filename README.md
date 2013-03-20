SimpleOrm
=========

simple data Orm based on PDO

# Installing/Configuring
```
phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make && make install
```
Then add
```
extension="SimpleOrm.so"
```
to your php.ini

#Usage

###SimpleOrm::getInstance($pdo)
```php
<?php
$dsn = "mysql:host=localhost;dbname=sakila";
$pdo= new PDO($dsn, 'root', '');
$obj=SimpleOrm::getInstance($pdo);
```