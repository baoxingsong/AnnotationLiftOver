/*
 * =====================================================================================
 *
 *       Filename:  alignNeedlemanForTranscript.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/02/2017 15:11:39
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Baoxing Song (songbx.me), songbaoxing168@163.com
 *
 * =====================================================================================
 */
#include <stdlib.h>
/*************************************************************************




 ************************************************************************/

#include <iostream>
#include <map>
#include "alignNeedlemanForTranscript.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "../util/parameters.h"
#include <algorithm>
SpliceSitePosition::SpliceSitePosition(int donorsSpliceSitePosition, int acceptorSpliceSitePosition) {
    this->_donorSpliceSitePosition=donorsSpliceSitePosition;
    this->_acceptorSpliceSitePosition=acceptorSpliceSitePosition;
}
int SpliceSitePosition::getDonorSpliceSitePosition(){
    return _donorSpliceSitePosition;
}

int SpliceSitePosition::getAcceptorSpliceSitePosition(){
    return _acceptorSpliceSitePosition;
}
NeedlemanWunschForTranscript::NeedlemanWunschForTranscript(std::string& dna_d, std::string& dna_q, int & startCodonPosition, int & stopCodonPosition,
                                                           std::vector<SpliceSitePosition>& splitSitePositions, std::map<std::string, std::string>& parameters, NucleotideCodeSubstitutionMatrix& nucleotideCodeSubstitutionMatrix){
    this->_dna_d = dna_d;
    this->_dna_q = dna_q;
    this->_exon_match_score = stoi(get_parameters("alignmentExonMatchP", parameters));
    this->_exon_mismatch_score = stoi(get_parameters("alignmentExonMismatchP", parameters));
    this->_exon_open_gap_penalty = stoi(get_parameters("alignmentExonOpenGapP", parameters));
    this->_exon_extend_gap_penalty = stoi(get_parameters("alignmentExonExtendGapP", parameters));

    this->_intron_match_score = stoi(get_parameters("alignmentIntronMatchP", parameters));
    this->_intron_mismatch_score  = stoi(get_parameters("alignmentIntronMismatchP", parameters));
    this->_intron_open_gap_penalty = stoi(get_parameters("alignmentIntronOpenGapP", parameters));
    this->_intron_extend_gap_penalty = stoi(get_parameters("alignmentIntronExtendGapP", parameters));

    this->_splice_sites_match_score = stoi(get_parameters("alignmentSpliceSitesMatchP", parameters));
    this->_splice_sites_mismatch_score = stoi(get_parameters("alignmentSpliceSitesMismatchP", parameters));
    this->_splice_sites_open_gap_penalty = stoi(get_parameters("alignmentSpliceSitesOpenGapP", parameters));
    this->_splice_sites_extend_gap_penalty = stoi(get_parameters("alignmentSpliceSitesExtendGapP", parameters));

    this->_start_stop_codon_match_score = stoi(get_parameters("alignmentStartStopCodonMatchP", parameters));
    this->_start_stop_codon_mismatch_score = stoi(get_parameters("alignmentStartStopCodonMismatchP", parameters));
    this->_start_stop_codon_open_gap_penalty = stoi(get_parameters("alignmentStartStopCodonOpenGapP", parameters));
    this->_start_stop_codon_extend_gap_penalty = stoi(get_parameters("alignmentStartStopCodonExtendGapP", parameters));
    this->_startCodonPosition = startCodonPosition;
    this->_stopCodonPosition = stopCodonPosition;
    this->_spliceSitePositions = splitSitePositions;
    this->_length_of_q = this->_dna_q.length();
    this->_length_of_d = this->_dna_d.length();
    this->_similarity_matrix = new int32_t*[this->_length_of_q + 1];
    for (int i = 0; i < (this->_length_of_q + 1); i++) {
        this->_similarity_matrix[i] = new int32_t[this->_length_of_d + 1];
    }
    for (int i = 0; i<= _length_of_q; i++) {
        for (int j = 0; j<= 1; j++) {
            _similarity_matrix[i][j] = 0;
        }
    }
    for (int j = 0; j<= _length_of_d; j++){
        for (int i = 0; i<= 1; i++) {
            _similarity_matrix[i][j] = 0;
        }
    }
    // this matrix is for set different penalty for open gap and extend gap begin
    // 0 for match, 1 for deletion, 2 for insertation
    // and the track also changed to use this matrix
    this->_track_matrix = new VARIANTCATEGORY*[this->_length_of_q + 1];
    for (int i = 0; i < (this->_length_of_q + 1); i++) {
        _track_matrix[i] = new VARIANTCATEGORY[this->_length_of_d + 1];
    }

    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j <= _length_of_d; j++) {
            _track_matrix[i][j] = SNPORINSERTIONORDELETION;
        }
    }
    for (int j = 0; j <= 1; j++){
        for (int i = 0; i <= _length_of_q; i++) {
            _track_matrix[i][j] = SNPORINSERTIONORDELETION;
        }
    }

    this->calculate_similarity(nucleotideCodeSubstitutionMatrix);
    this->get_optimal_alignment();
}

NeedlemanWunschForTranscript::~NeedlemanWunschForTranscript() {
    for (int i = 0; i <= _length_of_q; i++) {
        delete[] this->_similarity_matrix[i];
        delete[] this->_track_matrix[i];
    }
    delete[] this->_similarity_matrix;
    delete[] this->_track_matrix;
}

ELEMENTS NeedlemanWunschForTranscript::checkElements( int & position ){
    if( position < this->_startCodonPosition || position > this->_stopCodonPosition+2  ){
        return INTRON;
    }
    for(std::vector<SpliceSitePosition>::size_type i = 0; i != this->_spliceSitePositions.size(); i++ ){
    	SpliceSitePosition spliceSite = this->_spliceSitePositions[i];
        if(position > (spliceSite.getDonorSpliceSitePosition()+1) && position < (spliceSite.getAcceptorSpliceSitePosition()-1) ){
            return INTRON;
        }else if( position== spliceSite.getAcceptorSpliceSitePosition() || position == spliceSite.getAcceptorSpliceSitePosition()-1){
            return SPLICEACCEPTOR;
        }else if( position== spliceSite.getDonorSpliceSitePosition() || position == spliceSite.getDonorSpliceSitePosition()+1  ){
            return SPLICEDONOR;
        }
    }
    if (position == this->_startCodonPosition || position == this->_startCodonPosition+1 || position == this->_startCodonPosition+2 ){
        return START;
    }
    if (position == this->_stopCodonPosition || position == this->_stopCodonPosition+1 || position == this->_stopCodonPosition+2 ){
        return STOP;
    }
    return EXON;
}

// Calculating similarity matrix
void NeedlemanWunschForTranscript::calculate_similarity( NucleotideCodeSubstitutionMatrix& nucleotideCodeSubstitutionMatrix){

    int i;
    int match = 0, insert = 0, del = 0;
    for (int j=1; j < _length_of_d + 1; ++j){
        if( INTRON == this->checkElements(j) ) {
            for (i = 1; i < _length_of_q + 1; ++i) {
                match = _similarity_matrix[i - 1][j - 1] + nucleotideCodeSubstitutionMatrix.get_intron_subsitition_matrix()
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_q[i - 1])]
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_d[j - 1])];
                if (i==1 || _track_matrix[i - 1][j] == INSERTION || _track_matrix[i - 1][j] == SNPORINSERTION
                            || _track_matrix[i - 1][j] == SNPORINSERTIONORDELETION || _track_matrix[i - 1][j] == INSERTIONORDELETION ) { //deletion
                    insert = _similarity_matrix[i - 1][j] + this->_intron_extend_gap_penalty;
                } else {
                    insert = _similarity_matrix[i - 1][j] + this->_intron_open_gap_penalty;
                }
                if (j==1 || _track_matrix[i][j - 1] == DELETION || _track_matrix[i][j - 1] == SNPORDELETION ||
                            _track_matrix[i][j - 1] == SNPORINSERTIONORDELETION || _track_matrix[i][j-1] == INSERTIONORDELETION ) { //insertion
                    del = _similarity_matrix[i][j - 1] + this->_intron_extend_gap_penalty;
                } else {
                    del = _similarity_matrix[i][j - 1] + this->_intron_open_gap_penalty;
                }
                setScore( match, insert, del, i, j);
            }
        }else if( EXON == this->checkElements(j)  ){
            for (i = 1; i < _length_of_q + 1; ++i) {
                match = _similarity_matrix[i - 1][j - 1] + nucleotideCodeSubstitutionMatrix.get_exon_subsitition_matrix()
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_q[i - 1])]
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_d[j - 1])];
                if (i==1 || _track_matrix[i - 1][j] == INSERTION || _track_matrix[i - 1][j] == SNPORINSERTION
                            || _track_matrix[i - 1][j] == SNPORINSERTIONORDELETION || _track_matrix[i - 1][j] == INSERTIONORDELETION ) { //deletion
                    insert = _similarity_matrix[i - 1][j] + this->_exon_extend_gap_penalty;
                } else {
                    insert = _similarity_matrix[i - 1][j] + this->_exon_open_gap_penalty;
                }
                if (j==1 || _track_matrix[i][j - 1] == DELETION || _track_matrix[i][j - 1] == SNPORDELETION ||
                            _track_matrix[i][j - 1] == SNPORINSERTIONORDELETION || _track_matrix[i][j-1] == INSERTIONORDELETION ) { //insertion
                    del = _similarity_matrix[i][j - 1] + this->_exon_extend_gap_penalty;
                } else {
                    del = _similarity_matrix[i][j - 1] + this->_exon_open_gap_penalty;
                }
                setScore( match, insert, del, i, j);
            }
        }else if( START == this->checkElements(j) || STOP == this->checkElements(j)){
            for ( i = 1; i < _length_of_q + 1; ++i) {
                match = _similarity_matrix[i - 1][j - 1] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_q[i - 1])]
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_d[j - 1])];

                if (i==1 || _track_matrix[i - 1][j] == INSERTION || _track_matrix[i - 1][j] == SNPORINSERTION
                            || _track_matrix[i - 1][j] == SNPORINSERTIONORDELETION || _track_matrix[i - 1][j] == INSERTIONORDELETION ) { //deletion
                    insert = _similarity_matrix[i - 1][j] + this->_start_stop_codon_extend_gap_penalty;
                } else {
                    insert = _similarity_matrix[i - 1][j] + this->_start_stop_codon_open_gap_penalty;
                }
                if (j==1 || _track_matrix[i][j - 1] == DELETION || _track_matrix[i][j - 1] == SNPORDELETION ||
                            _track_matrix[i][j - 1] == SNPORINSERTIONORDELETION || _track_matrix[i][j-1] == INSERTIONORDELETION ) { //insertion
                    del = _similarity_matrix[i][j - 1] + this->_start_stop_codon_extend_gap_penalty;
                } else {
                    del = _similarity_matrix[i][j - 1] + this->_start_stop_codon_open_gap_penalty;
                }
                setScore( match, insert, del, i, j);
/*
                // test for new start and stop codon score method start
                if (j == _startCodonPosition + 2 && i > 2) {
                    std::stringstream targetthree;
                    targetthree << _dna_d[j - 3];
                    targetthree << _dna_d[j - 2];
                    targetthree << _dna_d[j - 1];
                    std::string startThree = targetthree.str();
                    if (nucleotideCodeSubstitutionMatrix.getPossibleStartCodons().find(startThree) !=
                        nucleotideCodeSubstitutionMatrix.getPossibleStartCodons().end()) {
                        _similarity_matrix[i - 2][j - 2] =
                                _similarity_matrix[i - 3][j - 3] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[0][0];
                        _track_matrix[i - 2][j - 2] = SNP;
                        _similarity_matrix[i - 1][j - 1] =
                                _similarity_matrix[i - 2][j - 2] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[0][0];
                        _track_matrix[i - 1][j - 1] = SNP;
                        _similarity_matrix[i][j] =
                                _similarity_matrix[i - 1][j - 1] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[0][0];
                        _track_matrix[i][j] = SNP;
                    }
                }

                if (j == _stopCodonPosition + 2 && i > 2) {
                    std::stringstream targetthree;
                    targetthree << _dna_d[j - 3];
                    targetthree << _dna_d[j - 2];
                    targetthree << _dna_d[j - 1];
                    std::string stopThree = targetthree.str();
                    // the the query has been the end, and the snp is not higher than insertion, do not change
                    if (nucleotideCodeSubstitutionMatrix.getPossibleStopCodons().find(stopThree) !=
                        nucleotideCodeSubstitutionMatrix.getPossibleStopCodons().end()
                        && !(i == _length_of_q &&
                             (_similarity_matrix[i - 3][j - 3] + 3 * nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[0][0]) <
                             _similarity_matrix[i][j])) {
                        _similarity_matrix[i - 2][j - 2] =
                                _similarity_matrix[i - 3][j - 3] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[0][0];
                        _track_matrix[i - 2][j - 2] = SNP;
                        _similarity_matrix[i - 1][j - 1] =
                                _similarity_matrix[i - 2][j - 2] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[0][0];
                        _track_matrix[i - 1][j - 1] = SNP;
                        _similarity_matrix[i][j] =
                                _similarity_matrix[i - 1][j - 1] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[0][0];
                        _track_matrix[i][j] = SNP;
                    }
                }
                // test for new start and stop codon score method end
                */
            }
        }else if ( SPLICEDONOR == this->checkElements(j) || SPLICEACCEPTOR == this->checkElements(j)  ){
            for (i = 1; i < _length_of_q + 1; ++i) {
                match = _similarity_matrix[i - 1][j - 1] + nucleotideCodeSubstitutionMatrix.get_splice_sites_subsitition_matrix()
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_q[i - 1] ) ]
                [nucleotideCodeSubstitutionMatrix.get_dna_acid_map(_dna_d[j - 1] ) ];
                if(i==1 ||  _track_matrix[i - 1][j] == INSERTION || _track_matrix[i - 1][j] == SNPORINSERTION
                            || _track_matrix[i - 1][j] == SNPORINSERTIONORDELETION || _track_matrix[i - 1][j] == INSERTIONORDELETION){ //deletion
                	insert = _similarity_matrix[i - 1][j] + this->_splice_sites_extend_gap_penalty;
            	}else{
                    insert = _similarity_matrix[i - 1][j] + this->_splice_sites_open_gap_penalty;
                }
            	if(j==1 || _track_matrix[i][j - 1] == DELETION || _track_matrix[i][j - 1] == SNPORDELETION ||
                           _track_matrix[i][j - 1] == SNPORINSERTIONORDELETION || _track_matrix[i][j-1] == INSERTIONORDELETION ){ //insertion
                	del = _similarity_matrix[i][j - 1] + this->_splice_sites_extend_gap_penalty;
            	}else{
                    del = _similarity_matrix[i][j - 1] + this->_splice_sites_open_gap_penalty;
                }
                setScore( match, insert, del, i, j);
            }
        }
    }
}

void NeedlemanWunschForTranscript::setScore(int32_t & match, int32_t & insert, int32_t & del, int & i, int & j){
    int32_t selected=0;
    if( del >insert && del==match  ){
        selected = del;
        _track_matrix[i][j] = SNPORDELETION;
    }else if( insert >del && insert == match  ){
        selected = match;
        _track_matrix[i][j] = SNPORINSERTION;
    }else if ( insert > match && insert > del){// prefer deletion
        int t = 1;
        while( i-t >=1 && (_track_matrix[i - t][j] == SNPORINSERTION || _track_matrix[i - t][j] == SNPORINSERTIONORDELETION || _track_matrix[i - t][j]==INSERTIONORDELETION ) ){
            _track_matrix[i - t][j] = INSERTION;
            ++t;
        }
        selected = insert;
        _track_matrix[i][j] = INSERTION;
    }else if( del > match && del > insert ){//prefer insertion, so that the INDELs could be put together
        int t = 1;
        while( j-t >=1 && (_track_matrix[i][j-t] == SNPORDELETION || _track_matrix[i][j-t] == SNPORINSERTIONORDELETION || _track_matrix[i][j-t]==INSERTIONORDELETION) ){
            _track_matrix[i][j-t] = DELETION;
            ++t;
        }
        selected = del;
        _track_matrix[i][j] = DELETION;
    }else if (match > insert && match > del){
        int t = 1;
        while( i-t >=1 && j-t>=1 && (_track_matrix[i-t][j-t] == SNPORINSERTION || _track_matrix[i-t][j-t] == SNPORINSERTIONORDELETION || _track_matrix[i-t][j-t]==SNPORDELETION ) ){
            _track_matrix[i-t][j-t] = SNP;
            ++t;
        }
        selected = match;
        _track_matrix[i][j] = SNP;
    }else if ( del >match && insert==del  ){
        selected = del;
        _track_matrix[i][j] = INSERTIONORDELETION;
    } else{
        selected = del;
        _track_matrix[i][j] = SNPORINSERTIONORDELETION;
    }
    _similarity_matrix[i][j] = selected;
}

//// Trace back step.
void NeedlemanWunschForTranscript::get_optimal_alignment() {
    _alignment_q = "";
    _alignment_d = "";
    std::stack<char> SQ, SD;

    int32_t k;
    size_t i = this->_length_of_q;
    size_t j = this->_length_of_d;
    int32_t highestScore = this->_similarity_matrix[i][j];
    for( k=_length_of_q; k >0; --k ){
        if( _similarity_matrix[k][j] > highestScore ){
            i = k;
            highestScore= _similarity_matrix[k][j];
        }
    }
    for( k=_length_of_q; k>i; --k ){
        SQ.push(_dna_q[k-1]);
        SD.push('-');
    }
    while (i > 0 || j > 0) {
        if (i == 0) {
            SQ.push('-');
            SD.push(_dna_d[j-1]);
            --j;
        } else if (j == 0) {
            SQ.push(_dna_q[i-1]);
            SD.push('-');
            --i;
        }else{
            if ( _track_matrix[i][j]==SNP || _track_matrix[i][j]==SNPORDELETION || _track_matrix[i][j]==SNPORINSERTION || _track_matrix[i][j]==SNPORINSERTIONORDELETION  ) {
                SQ.push(_dna_q[i - 1]);
                SD.push(_dna_d[j - 1]);
                --i;
                --j;
            } else if (_track_matrix[i][j]==DELETION || _track_matrix[i][j]==INSERTIONORDELETION) {// Going to S(i, j-1) //deletion
                SQ.push('-');
                SD.push(_dna_d[j - 1]);
                --j;
            } else {        //insertion
                SQ.push(_dna_q[i - 1]);
                SD.push('-');
                --i;
            }
        }
    }
    while (!SQ.empty()) {
        _alignment_q += SQ.top();
        _alignment_d += SD.top();
        SQ.pop();
        SD.pop();
    }
}

std::string NeedlemanWunschForTranscript::getAlignment_q(){
    return _alignment_q;
}
std::string NeedlemanWunschForTranscript::getAlignment_d(){
    return _alignment_d;
}
void NeedlemanWunschForTranscript::print_results() {
    std::cout << _alignment_q << std::endl;
    std::cout << _alignment_d << std::endl;
}












int checkElements_temp( const int & startCodonPosition, const int & stopCodonPosition, const std::vector<SpliceSitePosition>& spliceSitePositions, const int & position ){
    if( position < startCodonPosition || position > stopCodonPosition+2  ){
        return 1;//INTRON;
    }
    for(std::vector<SpliceSitePosition>::size_type i = 0; i != spliceSitePositions.size(); i++ ){
        SpliceSitePosition spliceSite = spliceSitePositions[i];
        if(position > (spliceSite.getDonorSpliceSitePosition()+1) && position < (spliceSite.getAcceptorSpliceSitePosition()-1) ){
            return 1;//INTRON;
        }else if( position== spliceSite.getAcceptorSpliceSitePosition() || position == spliceSite.getAcceptorSpliceSitePosition()-1){
            return 2;//SPLICEACCEPTOR;
        }else if( position== spliceSite.getDonorSpliceSitePosition() || position == spliceSite.getDonorSpliceSitePosition()+1  ){
            return 2;//SPLICEDONOR;
        }
    }
    if (position == startCodonPosition || position == startCodonPosition+1 || position == startCodonPosition+2 ){
        return 3;//START;
    }
    if (position == stopCodonPosition || position == stopCodonPosition+1 || position == stopCodonPosition+2 ){
        return 3;//STOP;
    }
    return 0;//EXON;
}

static const int32_t SCORE_OUT_BANDED_ALIGNMENT_REGION = -536870912;

// this is the semiglobal weighted sequence alignment approach
// similar with the above function by use single gap penalty
void alignNeedlemanForTranscript(std::string& dna_d,
        std::string& dna_q, int & startCodonPosition, int & stopCodonPosition,
        std::vector<SpliceSitePosition>& spliceSitePositions,
        std::map<std::string, std::string>& parameters,
        NucleotideCodeSubstitutionMatrix& nucleotideCodeSubstitutionMatrix,
        std::string & alignment_q, std::string & alignment_d, std::string & infor){

    int32_t * _match_score = new int32_t[4];
    int32_t * _mis_match_score = new int32_t[4];
    _match_score[0] = stoi(get_parameters("alignmentExonMatchP", parameters));
    _mis_match_score[0] = stoi(get_parameters("alignmentExonMismatchP", parameters));

    _match_score[1] = stoi(get_parameters("alignmentIntronMatchP", parameters));
    _mis_match_score[1] = stoi(get_parameters("alignmentIntronMismatchP", parameters));

    _match_score[2] = stoi(get_parameters("alignmentSpliceSitesMatchP", parameters));
    _mis_match_score[2] = stoi(get_parameters("alignmentSpliceSitesMismatchP", parameters));

    _match_score[3] = stoi(get_parameters("alignmentStartStopCodonMatchP", parameters));
    _mis_match_score[3] = stoi(get_parameters("alignmentStartStopCodonMismatchP", parameters));


    int32_t * _open_gap_penaltys = new int32_t[4];
    int32_t * _extend_gap_penaltys = new int32_t[4];

    _open_gap_penaltys[0]=stoi(get_parameters("alignmentExonOpenGapP", parameters));
    _extend_gap_penaltys[0]=stoi(get_parameters("alignmentExonExtendGapP", parameters));

    _open_gap_penaltys[1]=stoi(get_parameters("alignmentIntronOpenGapP", parameters));
    _extend_gap_penaltys[1]=stoi(get_parameters("alignmentIntronExtendGapP", parameters));

    _open_gap_penaltys[2]=stoi(get_parameters("alignmentSpliceSitesOpenGapP", parameters));
    _extend_gap_penaltys[2]=stoi(get_parameters("alignmentSpliceSitesExtendGapP", parameters));

    _open_gap_penaltys[3]=stoi(get_parameters("alignmentStartStopCodonOpenGapP", parameters));
    _extend_gap_penaltys[3]=stoi(get_parameters("alignmentStartStopCodonExtendGapP", parameters));


    size_t length_of_q = dna_q.length();
    size_t length_of_d = dna_d.length();

    //check potential overflow problem begin
    int32_t maxLength = length_of_d;

    if( maxLength <  length_of_q){
        maxLength = length_of_q;
    }
    int32_t maxScore = _match_score[0];

    int32_t i, j, ai, k;
    for( i=1; i<4; ++i ){
        if( maxScore < _match_score[i] ){
            maxScore =  _match_score[i];
        }
    }

    if( (maxLength * maxScore) > pow(2,31)){
        std::cerr << "the sequence is too long, could not be optimized with this pipeline" << std::endl;
        return;
    }
    int32_t minimumPenalty = _mis_match_score[0];
    for( i=1; i<4; ++i ){
        if( minimumPenalty > _mis_match_score[i] ){
            minimumPenalty = _mis_match_score[i];
        }
    }

    for( i=0; i<4; ++i ){
        if( minimumPenalty > _open_gap_penaltys[i] ){
            minimumPenalty =  _open_gap_penaltys[i];
        }
    }
    if( (maxLength * minimumPenalty) < -pow(2,31)){
        std::cerr << "the sequence is too long, could not be optimized with this pipeline" << std::endl;
        return;
    } //check potential overflow problem end

    for( i=0; i<4; ++i ){
        if( _open_gap_penaltys[i] > _extend_gap_penaltys[i] ){
            std::cerr << "it is not reasonable to have open gap penalty larger than extend gap penalty" << std::endl;
        }
    }//warning message about score strategy end

    int32_t  mscore;
    uint8_t d;
    Matrix T(length_of_d + 1, length_of_q + 1);
    int32_t * t;
    int32_t * M1 = new int32_t [length_of_q + 1]; //M1 and M2 is for the previous column and the current column
    int32_t * M2 = new int32_t [length_of_q + 1];

    int32_t * F = new int32_t [length_of_q + 1]; //F

    std::fill_n(M1, length_of_q+1, SCORE_OUT_BANDED_ALIGNMENT_REGION);
    std::fill_n(M2, length_of_q+1, SCORE_OUT_BANDED_ALIGNMENT_REGION);
    std::fill_n(F, length_of_q+1, SCORE_OUT_BANDED_ALIGNMENT_REGION);
    M1[0]=0;
    M2[0]=0;

    int32_t e, h, f, e1, zdrop, _open_gap_penalty1, _extend_gap_penalty1;
    int32_t ** m;

//    int32_t match_score;
//    int32_t mis_match_score;
    int32_t _open_gap_penalty;
    int32_t _extend_gap_penalty;
    for ( i=1; i<=length_of_d; ++i ){
        e = SCORE_OUT_BANDED_ALIGNMENT_REGION;
        int8_t ref_element = checkElements_temp(startCodonPosition, stopCodonPosition, spliceSitePositions, i + 1);
//        match_score = _match_score[ref_element];
//        mis_match_score = _mis_match_score[ref_element];
        _open_gap_penalty = _open_gap_penaltys[ref_element];
        _extend_gap_penalty = _extend_gap_penaltys[ref_element];

        for (j = 1; j <= length_of_q; ++j) {
            if( ref_element == 0 ) { //exon
                mscore = M1[j - 1] + nucleotideCodeSubstitutionMatrix.get_exon_subsitition_matrix()[ai][nucleotideCodeSubstitutionMatrix.get_dna_acid_map( dna_q[j])];
            }else if ( ref_element == 1 ) { //exon
                mscore = M1[j - 1] + nucleotideCodeSubstitutionMatrix.get_intron_subsitition_matrix()[ai][nucleotideCodeSubstitutionMatrix.get_dna_acid_map(dna_q[j])];
            }else if ( ref_element == 2 ) { //splice
                mscore = M1[j - 1] + nucleotideCodeSubstitutionMatrix.get_splice_sites_subsitition_matrix()[ai][nucleotideCodeSubstitutionMatrix.get_dna_acid_map(dna_q[j])];
            }else{
                mscore = M1[j - 1] + nucleotideCodeSubstitutionMatrix.get_start_stop_codon_subsitition_matrix()[ai][nucleotideCodeSubstitutionMatrix.get_dna_acid_map(dna_q[j])];
            }

            d = mscore > F[j] ? 0 : 1;
            mscore = mscore > F[j] ? mscore : F[j];

            d = mscore > e ? d : 2;
            mscore = mscore > e ? mscore : e;

            M2[j] = mscore;

            h = mscore + _open_gap_penalty;
            f = F[j] + _extend_gap_penalty;
            d |= f >= h ? 1 << 3 : 0;
            f = f >= h ? f : h;
            F[j] = f;

            e += _extend_gap_penalty;
            d |= e >= h ? 1 << 4 : 0;
            e = e >= h ? e : h;

            T.set(i, j, d);


        }
        t = M1;
        M1 = M2;
        M2 = t;
    }

    std::vector<uint32_t> cigar;

    uint32_t op = 0;
    uint32_t length = 1;
    // trace back begin
    // For speed up and RAM saving purpose, this is just am approximation tracing back implementation
    int32_t ii = length_of_d;
    int32_t jj = length_of_q;
    int tmp, state=0;
    while (ii>0 && jj>0) {
        tmp = T.get(ii, jj);
        if (state == 0) state = tmp & 7; // if requesting the H state, find state one maximizes it.
        else if (!(tmp >> (state + 2) & 1)) state = 0; // if requesting other states, _state_ stays the same if it is a continuation; otherwise, set to H
        if (state == 0) state = tmp & 7;
        if (state == 0){
            op=0;
            --ii;
            --jj;
        }else if (state == 1 || state == 3){
            op =2;
            --ii;
        }else{
            op =1;
            --jj;
        }

        if( cigar.size() == 0 || op != (cigar[cigar.size() - 1]&0xf) ){
            cigar.push_back(length << 4 | op);
        }else{
            cigar[cigar.size()-1] += length<<4;
        }
    }
    while( ii>0 ){
        op =2;
        --ii;
        if( cigar.size() == 0 || op != (cigar[cigar.size() - 1]&0xf) ){
            cigar.push_back(length << 4 | op);
        }else{
            cigar[cigar.size()-1] += length<<4;
        }
    }
    while( jj>0 ){
        op =1;
        --jj;
        if( cigar.size() == 0 || op != (cigar[cigar.size() - 1]&0xf) ){
            cigar.push_back(length << 4 | op);
        }else{
            cigar[cigar.size()-1] += length<<4;
        }
    }

    int32_t refPosition = 0;
    int32_t queryPostion = 0;

    for( i=cigar.size()-1; i >= 0; --i ){
        uint32_t cigarLength = cigar[i]>>4;
        uint32_t cigarType = cigar[i]&0xf;
        // "MID"
        if( 0 == cigarType ){
            alignment_d += dna_d.substr( refPosition, cigarLength );
            alignment_q += dna_q.substr( queryPostion, cigarLength );
            refPosition += cigarLength;
            queryPostion += cigarLength;
        } else if ( 1 == cigarType ){
            alignment_d += std::string(cigarLength, '-');
            alignment_q += dna_q.substr( queryPostion, cigarLength );
            queryPostion += cigarLength;
        }else if (2 == cigarType){
            alignment_d += dna_d.substr( refPosition, cigarLength );
            alignment_q += std::string(cigarLength, '-');
            refPosition += cigarLength;
        }else{
            std::cout << "some thing unknown happened with the cigar vector parser" << std::endl;
        }
    }
}
