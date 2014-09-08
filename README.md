# Pihsi

[![Gem Version](https://badge.fury.io/rb/pihsi.svg)](http://badge.fury.io/rb/pihsi)
[![Dependency Status](https://gemnasium.com/siegfried/pihsi.svg)](https://gemnasium.com/siegfried/pihsi)
[![Build Status](https://travis-ci.org/siegfried/pihsi.svg?branch=master)](https://travis-ci.org/siegfried/pihsi)
[![Coverage Status](https://coveralls.io/repos/siegfried/pihsi/badge.png)](https://coveralls.io/r/siegfried/pihsi)
[![Inline docs](http://inch-ci.org/github/siegfried/pihsi.svg?branch=master)](http://inch-ci.org/github/siegfried/pihsi)

Pihsi is a Ruby Speech Recognition toolkit based on [PocketSphinx](http://cmusphinx.sourceforge.net).

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'pihsi'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install pihsi

## Usage

Initialize Pihsi::SpeechRecognizer with proper [hmm, lm and dict](http://cmusphinx.sourceforge.net/wiki/tutorialpocketsphinx#initialization):

```ruby
recognizer = Pihsi::SpeechRecognizer.new hmm: 'xxx', lm: 'yyy', dict: 'zzz'
```

Recognize a string read from your audio file or an object respond to #read (IO object):

```ruby
recognizer.recognize data
```

## Contributing

1. Fork it ( https://github.com/siegfried/pihsi/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
