require "pocket_sphinx/pocket_sphinx"

module Pihsi
  class SpeechRecognizer
    attr_reader :decoder

    def initialize(hmm, lm, dict)
      @decoder = PocketSphinx::Decoder.new hmm, lm, dict
    end

    def recognize(data)
      decoder.decode data.unpack('s*')
    end
  end
end
