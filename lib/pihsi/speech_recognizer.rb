require "pocket_sphinx/pocket_sphinx"

module Pihsi
  class SpeechRecognizer
    include PocketSphinx
    attr_reader :hmm, :lm, :dict

    def initialize(hmm, lm, dict)
      @hmm, @lm, @dict = hmm, lm, dict
    end

    def recognize(data)
      super data.unpack('s*')
    end
  end
end
