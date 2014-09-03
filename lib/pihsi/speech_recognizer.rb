require "pocket_sphinx/pocket_sphinx"

module Pihsi
  class SpeechRecognizer
    attr_reader :decoder

    def initialize(options = {})
      _options = options.inject({}) do |result, (key, value)|
        result["-#{key}"] = value unless value.nil?
        result
      end.to_a
      @decoder = PocketSphinx::Decoder.new(_options)
    end

    def recognize(data)
      decoder.decode data.unpack('s*')
    end
  end
end
