RSpec.describe Pihsi::SpeechRecognizer do
  let(:instance) { build :speech_recognizer }

  describe '#recognize(data)' do
    subject { instance.recognize(data) }

    context 'when data is the content of goforward.raw' do
      let(:data) { File.read('spec/fixtures/goforward.raw') }

      it { should eq "go forward ten meters" }
    end

    context 'when data is an IO object of goforward.raw' do
      let(:data) { File.open('spec/fixtures/goforward.raw') }

      it { should eq "go forward ten meters" }
    end

    context 'when data is invalid' do
      let(:data) { 1 }

      specify { expect { subject }.to raise_error(ArgumentError) }
    end
  end
end
