require "pocket_sphinx/pocket_sphinx"

module Pihsi
  class SpeechRecognizer
    attr_reader :decoder

    def initialize(options = {})
      _options = options.inject([]) do |result, (key, value)|
        result << ["-#{key}", value] unless value.nil?
        result
      end
      @decoder = PocketSphinx::Decoder.new(_options)
    end

    # Converts raw audio data into text
    # @param data [String] the raw audio data
    # @return [String, nil] the transcribed text or nil
    def recognize(data)
      if result = decoder.decode(data.unpack('s*'))
        result["hypothesis"]
      end
    end
  end
end
